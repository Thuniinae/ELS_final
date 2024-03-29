#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

#include "Testbench.h"


unsigned char header[54] = {
    0x42,          // identity : B
    0x4d,          // identity : M
    0,    0, 0, 0, // file size
    0,    0,       // reserved1
    0,    0,       // reserved2
    54,   0, 0, 0, // RGB data offset
    40,   0, 0, 0, // struct BITMAPINFOHEADER size
    0,    0, 0, 0, // bmp width
    0,    0, 0, 0, // bmp height
    1,    0,       // planes
    24,   0,       // bit per pixel
    0,    0, 0, 0, // compression
    0,    0, 0, 0, // data size
    0,    0, 0, 0, // h resolution
    0,    0, 0, 0, // v resolution
    0,    0, 0, 0, // used colors
    0,    0, 0, 0  // important colors
};

Testbench::Testbench(sc_module_name n) : sc_module(n), output_rgb_raw_data_offset(54) {
  SC_THREAD(feed_rgb);
  sensitive << i_clk.pos();
  dont_initialize();
  SC_THREAD(fetch_result);
  sensitive << i_clk.pos();
  dont_initialize();
}

Testbench::~Testbench() {
	cout << "Total run time = " << total_run_time << endl;
}

int Testbench::read_bmp(string infile_name) {
  FILE *fp_s = NULL; // source file handler
  fp_s = fopen(infile_name.c_str(), "rb");
  if (fp_s == NULL) {
    printf("fopen %s error\n", infile_name.c_str());
    return -1;
  }
  // move offset to 10 to find rgb raw data offset
  fseek(fp_s, 10, SEEK_SET);
  fread(&input_rgb_raw_data_offset, sizeof(unsigned int), 1, fp_s);

  // move offset to 18 to get width & height;
  fseek(fp_s, 18, SEEK_SET);
  fread(&width, sizeof(unsigned int), 1, fp_s);
  fread(&height, sizeof(unsigned int), 1, fp_s);

  // get bit per pixel
  fseek(fp_s, 28, SEEK_SET);
  fread(&bits_per_pixel, sizeof(unsigned short), 1, fp_s);
  bytes_per_pixel = bits_per_pixel / 8;

  // move offset to input_rgb_raw_data_offset to get RGB raw data
  fseek(fp_s, input_rgb_raw_data_offset, SEEK_SET);

  source_bitmap =
      (unsigned char *)malloc((size_t)width * height * bytes_per_pixel);
  if (source_bitmap == NULL) {
    printf("malloc images_s error\n");
    return -1;
  }

  target_bitmap =
      (unsigned char *)malloc((size_t)width * height * bytes_per_pixel);
  if (target_bitmap == NULL) {
    printf("malloc target_bitmap error\n");
    return -1;
  }

  fread(source_bitmap, sizeof(unsigned char),
        (size_t)(long)width * height * bytes_per_pixel, fp_s);
  fclose(fp_s);
  return 0;
}

int Testbench::write_bmp(string outfile_name) {
  FILE *fp_t = NULL;      // target file handler
  unsigned int file_size; // file size

  fp_t = fopen(outfile_name.c_str(), "wb");
  if (fp_t == NULL) {
    printf("fopen %s error\n", outfile_name.c_str());
    return -1;
  }

  // file size
  file_size = width * height * bytes_per_pixel + output_rgb_raw_data_offset;
  header[2] = (unsigned char)(file_size & 0x000000ff);
  header[3] = (file_size >> 8) & 0x000000ff;
  header[4] = (file_size >> 16) & 0x000000ff;
  header[5] = (file_size >> 24) & 0x000000ff;

  // width
  header[18] = width & 0x000000ff;
  header[19] = (width >> 8) & 0x000000ff;
  header[20] = (width >> 16) & 0x000000ff;
  header[21] = (width >> 24) & 0x000000ff;

  // height
  header[22] = height & 0x000000ff;
  header[23] = (height >> 8) & 0x000000ff;
  header[24] = (height >> 16) & 0x000000ff;
  header[25] = (height >> 24) & 0x000000ff;

  // bit per pixel
  header[28] = bits_per_pixel;

  // write header
  fwrite(header, sizeof(unsigned char), output_rgb_raw_data_offset, fp_t);

  // write image
  fwrite(target_bitmap, sizeof(unsigned char),
         (size_t)(long)width * height * bytes_per_pixel, fp_t);

  fclose(fp_t);
  return 0;
}

////
// Convert a time in simulation time units to clock cycles
////
int Testbench::clock_cycle( sc_time time ) {
    sc_clock * clk_p = dynamic_cast < sc_clock * >( i_clk.get_interface() );
    sc_time clock_period = clk_p->period(); // get period from the sc_clock object.
    return ( int )( time / clock_period );

}

void Testbench::write(sc_uint<24> rgb, int channel) {
  switch(channel){
    case 1:
      #ifndef NATIVE_SYSTEMC
        o_rgbTG.put(rgb);
      #else
        o_rgbTG.write(rgb);
        wait(1); //emulate channel delay
      #endif
      break;
    case 2:
      #ifndef NATIVE_SYSTEMC
        o_rgbTN.put(rgb);
      #else
        o_rgbTN.write(rgb);
        wait(1); //emulate channel delay
      #endif
      break;
    default:
      cout << "unknown rgb channel!" << endl;
      break;
  }
}

sc_uint<24> Testbench::pixel(unsigned int i, unsigned int j) {
  unsigned char R, G, B;      // color of R, G, B
  if (i >= 0 && i < width && j >= 0 && j < height) {
    R = *(source_bitmap +
          bytes_per_pixel * (width * j + i) + 2);
    G = *(source_bitmap +
          bytes_per_pixel * (width * j + i) + 1);
    B = *(source_bitmap +
          bytes_per_pixel * (width * j + i) + 0);
  } else {
    std::cout << "caution, pixel out of range" << std::endl;
    R = 0;
    G = 0;
    B = 0;
  }
  sc_dt::sc_uint<24> rgb;
  rgb.range(7, 0) = R;
  rgb.range(15, 8) = G;
  rgb.range(23, 16) = B;
  return rgb;
}

void Testbench::write_mean(sc_biguint<192> mean, int channel){
  switch(channel){
    case 1:
      #ifndef NATIVE_SYSTEMC
        o_meanTG.put(mean);
      #else
        o_meanTG.write(mean);
        wait(1); //emulate channel delay
      #endif
      break;
    case 2:
      #ifndef NATIVE_SYSTEMC
        o_meanTC.put(mean);
      #else
        o_meanTC.write(mean);
        wait(1); //emulate channel delay
      #endif
      break;
    default:
      cout << "unknown mean channel!" << endl;
      break;
  }
}

sc_biguint<192> Testbench::read_mean() {
  sc_biguint<192> mean;
  #ifndef NATIVE_SYSTEMC
    mean = i_meanNT.get();
  #else
    mean = i_meanNT.read();
  #endif
  return mean;
}

sc_uint<3> Testbench::read_index() {
  sc_uint<3> index;
  #ifndef NATIVE_SYSTEMC
    index = i_indexAT.get();
  #else
    index = i_indexAT.read();
  #endif
  return index;
}

void Testbench::write_index(sc_uint<3> index, int channel) {
  switch(channel){
    case 1:
      #ifndef NATIVE_SYSTEMC
        o_indexTN.put(index);
      #else
        o_indexTN.write(index);
        wait(1); //emulate channel delay
      #endif
      break;
    case 2:
      #ifndef NATIVE_SYSTEMC
        o_indexTC.put(index);
      #else
        o_indexTC.write(index);
        wait(1); //emulate channel delay
      #endif
      break;
    default:
      cout << "unknown mean channel!" << endl;
      break;
  }
}


void Testbench::feed_rgb() {
  #ifndef NATIVE_SYSTEMC
    o_rgbTG.reset();
    o_rgbTN.reset();
	  o_meanTG.reset();
	  o_meanTC.reset();
    o_indexTN.reset();
    o_indexTC.reset();
    i_indexAT.reset();
	  i_meanNT.reset();
  #endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);
	total_start_time = sc_time_stamp();

  // send initial K-means, K = 8
  sc_biguint<192> mean;
  
  // select initial mean
  /*for (int i = 0; i < 8; i++) {
    mean.range(i * 24 + 7, i * 24) = pixel(rand()%width, rand()%height);
  }*/
  mean.range(23, 0) = pixel(width>>1, height>>1);
  mean.range(47, 24) = pixel(width>>2, height>>1);
  mean.range(71, 48) = pixel(width>>1, height>>2);
  mean.range(95, 72) = pixel(width>>2, height>>2);
  mean.range(119, 96) = pixel((width>>1) + (width>>2), (height>>1) + (height>>2));
  mean.range(143, 120) = pixel((width>>1) + (width>>2), height>>1);
  mean.range(167, 144) = pixel(width>>1, (height>>1) + (height>>2));
  mean.range(191, 168) = pixel((width>>1) + (width>>2), (height>>1) - (height>>2));
  int cnt = 0;
  sc_time t1;
  for(int k = 0; k < 30; k++) { // send sampled pixels until converge (maximum 30 times)
    for (unsigned int x = 0; x < width; x = x + 16) {
      for (unsigned int y = 0; y < height; y = y + 16) {
        sc_time t0 = sc_time_stamp();
        write(pixel(x , y), 1);
        write_mean(mean, 1);
        sc_uint<3> index = read_index();
        t1 = sc_time_stamp();
        cout << "GD_AM latency:" << clock_cycle(t1 - t0) << endl;
        write(pixel(x , y), 2);
        write_index(index, 1);
      }
    }
    sc_biguint<192> new_mean;
    new_mean = read_mean();
    cout << "NM last rgb latency:" << clock_cycle(sc_time_stamp() - t1) << endl;
    /*cout << cnt << endl;
    cout << "means:" << endl;
		for (int i = 0; i < 8; i++){
			for (int j =0; j < 3; j++) { // for R, G, B
				cout << mean.range((i * 24) + (j<<3) + 7, (i * 24) + (j<<3)) << ", ";
			}
			cout << endl;
		}
    cout << "new means:" << endl;
		for (int i = 0; i < 8; i++){
			for (int j =0; j < 3; j++) { // for R, G, B
				cout << new_mean.range((i * 24) + (j<<3) + 7, (i * 24) + (j<<3)) << ", ";
			}
			cout << endl;
		}*/
    if (!converge(mean, new_mean, 1)) // if not converge
      mean = new_mean;
    else {
      mean = new_mean;
      break;
    }
    cnt++;
  }
  // send all the pixels for coloring
  for (unsigned int x = 0; x < width; x++) {
    for (unsigned int y = 0; y < height; y++) {
      sc_time t0 = sc_time_stamp();
      write(pixel(x , y), 1);
      write_mean(mean, 1);
      sc_uint<3> index = read_index();
      cout << "GD-AM-2 latency: " << clock_cycle(sc_time_stamp() - t0) << endl;
      time_queue.push( sc_time_stamp() );
      write_index(index, 2);
      write_mean(mean, 2);
    }
  }
}

void Testbench::fetch_result() {
  unsigned char R, G, B;      // color of R, G, B
#ifndef NATIVE_SYSTEMC
	i_result.reset();
#endif
	wait(5);
	wait(1);

  // read the processed image
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      sc_uint<24> rgb;
#ifndef NATIVE_SYSTEMC
      rgb = i_result.get();
#else
      rgb = i_result.read();
#endif
      sc_time t0( time_queue.front() );
      time_queue.pop();    
      cout << "CP latency: " << clock_cycle(sc_time_stamp() - t0) << endl;
      R = rgb.range(7, 0);
      G = rgb.range(15, 8);
      B = rgb.range(23, 16);
      *(target_bitmap + bytes_per_pixel * (width * y + x) + 2) = R;
      *(target_bitmap + bytes_per_pixel * (width * y + x) + 1) = G;
      *(target_bitmap + bytes_per_pixel * (width * y + x) + 0) = B;
    }
  }
  std::cout << "latency:" << clock_cycle(sc_time_stamp() - total_start_time) << std::endl;
	total_run_time = sc_time_stamp() - total_start_time;
  sc_stop();
}

bool Testbench::converge (sc_biguint<192> mean1, sc_biguint<192> mean2, sc_uint<8> threshold) {
  bool result;
  sc_uint<16> error;
  // mean square error for 8 means
  result = true;
  for (int i = 0; i < K; i++) { // for K-means
    sc_uint<24> m1, m2;
    m1 = mean1.range(i * 24 + 23, i * 24);
    m2 = mean2.range(i * 24 + 23, i * 24);
    error = 0; // initialize
    for (int j = 0; j < 3; j++){ // for R, G, B
      error += pow(m1.range((j<<3) + 7, j<<3) - m2.range((j<<3) + 7, j<<3), 2);
    }
    if (error > threshold * threshold) {
      result = false;
      break;
    }
  }
  return result;
}

