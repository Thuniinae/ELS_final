//******************************************************************************
// Copyright 2015 Cadence Design Systems, Inc.
// All Rights Reserved.
//
//******************************************************************************

#ifndef _BMP_HEADER_H_
#define _BMP_HEADER_H_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class bmp_data
{
    public:
        struct file_header_dt
        {
            unsigned short bfType;
            unsigned int  bfSize;
            unsigned short bfReserved1;
            unsigned short bfReserved2;
            unsigned int  bfOffBytes;

            file_header_dt(){bfSize = 0;}

            void read(ifstream& in_file)
            {
                in_file.read((char *)(&bfType), sizeof(bfType));
                in_file.read((char *)(&bfSize), sizeof(bfSize));
                in_file.read((char *)(&bfReserved1), sizeof(bfReserved1));
                in_file.read((char *)(&bfReserved2), sizeof(bfReserved2));
                in_file.read((char *)(&bfOffBytes), sizeof(bfOffBytes));
            }

            void write(ofstream& out_file)
            {
                out_file.write((char *)(&bfType), sizeof(bfType));
                out_file.write((char *)(&bfSize), sizeof(bfSize));
                out_file.write((char *)(&bfReserved1), sizeof(bfReserved1));
                out_file.write((char *)(&bfReserved2), sizeof(bfReserved2));
                out_file.write((char *)(&bfOffBytes), sizeof(bfOffBytes));
            }

            void print()
            {
//                cout << hex;  
                cout << " bfType:      " << bfType << endl;
                cout << " bfSize:      " << bfSize << endl;
                cout << " bfReserved1: " << bfReserved1 << endl;
                cout << " bfReserved2: " << bfReserved2 << endl;
                cout << " bfOffBytes:  " << bfOffBytes << endl;
            }

        }; //file_header_dt

        struct info_header_dt
        {
            unsigned int  biSize;
            unsigned int  biWidth;
            unsigned int  biHeight;
            unsigned short biPlanes;
            unsigned short biBitCount;
            unsigned int  biCompression;
            unsigned int  biSizeImage;
            unsigned int  biXPelsPerMeter;
            unsigned int  biYPelsPerMeter;
            unsigned int  biClrUsed;
            unsigned int  biClrImportant;

            info_header_dt() 
            {
                biWidth 
                = biHeight
                = biCompression
                = biSizeImage
                = biXPelsPerMeter
                = biYPelsPerMeter
                = biClrUsed
                = biClrImportant
                = 0; 
        
                biSize = sizeof (info_header_dt);
        
                biPlanes 
                = biBitCount 
                = 0;
            }

            void read(ifstream& in_file)
            {
//	        printf("@@@@#### file read\n");
                in_file.read((char *)(&biSize), sizeof(biSize));
                in_file.read((char *)(&biWidth), sizeof(biWidth));
                in_file.read((char *)(&biHeight), sizeof(biHeight));
                in_file.read((char *)(&biPlanes), sizeof(biPlanes));
                in_file.read((char *)(&biBitCount), sizeof(biBitCount));
                in_file.read((char *)(&biCompression), sizeof(biCompression));
                in_file.read((char *)(&biSizeImage), sizeof(biSizeImage));
                in_file.read((char *)(&biXPelsPerMeter), sizeof(biXPelsPerMeter));
                in_file.read((char *)(&biYPelsPerMeter), sizeof(biYPelsPerMeter));
                in_file.read((char *)(&biClrUsed), sizeof(biClrUsed));
                in_file.read((char *)(&biClrImportant), sizeof(biClrImportant));
            }

            void write(ofstream& out_file)
            {
                out_file.write((char *)(&biSize), sizeof(biSize));
                out_file.write((char *)(&biWidth), sizeof(biWidth));
                out_file.write((char *)(&biHeight), sizeof(biHeight));
                out_file.write((char *)(&biPlanes), sizeof(biPlanes));
                out_file.write((char *)(&biBitCount), sizeof(biBitCount));
                out_file.write((char *)(&biCompression), sizeof(biCompression));
                out_file.write((char *)(&biSizeImage), sizeof(biSizeImage));
                out_file.write((char *)(&biXPelsPerMeter), sizeof(biXPelsPerMeter));
                out_file.write((char *)(&biYPelsPerMeter), sizeof(biYPelsPerMeter));
                out_file.write((char *)(&biClrUsed), sizeof(biClrUsed));
                out_file.write((char *)(&biClrImportant), sizeof(biClrImportant));
            }

            void print()
            {
//                cout << hex;  
                cout << " biSize:           " << biSize << endl;
                cout << " biWidth:          " << biWidth << endl;
                cout << " biHeight:         " << biHeight << endl;
                cout << " biPlanes:         " << biPlanes << endl;
                cout << " biBitCount:       " << biBitCount << endl;
                cout << " biCompression:    " << biCompression << endl;
                cout << " biSizeImage:      " << biSizeImage << endl;
                cout << " biXPelsPerMeter:  " << biXPelsPerMeter << endl;
                cout << " biYPelsPerMeter:  " << biYPelsPerMeter << endl;
                cout << " biClrUsed:        " << biClrUsed << endl;
                cout << " biClrImportant:   " << biClrImportant << endl;
            }

        }; //info_header_dt

        char * bfName;
        file_header_dt fhdt;
        info_header_dt ihdt;

        const unsigned int FILE_HEADER_LEN;
        const unsigned int INFO_HEADER_LEN;

        bmp_data(char * name="",const unsigned int fl=14, const unsigned int il=40)
            :bfName(name)
            ,FILE_HEADER_LEN(fl)
            ,INFO_HEADER_LEN(il)
            {
                ;
            }

        void read_file_header(ifstream& in_file)
        {
            in_file.seekg(0, ios::beg);
            fhdt.read(in_file);
        }

        void write_file_header(ofstream& out_file)
        {
            out_file.seekp(0, ios::beg);
            fhdt.write(out_file);
        }

        void read_info_header(ifstream& in_file)
        {
            in_file.seekg(FILE_HEADER_LEN, ios::beg);
            ihdt.read(in_file);
        }

        void write_info_header(ofstream& out_file)
        {
            out_file.seekp(FILE_HEADER_LEN, ios::beg);
            ihdt.write(out_file);
        }

        void set_width(unsigned int w)
        {
            ihdt.biWidth = w;
            ihdt.biSizeImage = ihdt.biHeight*ihdt.biWidth*(ihdt.biBitCount/8);
            fhdt.bfSize = ihdt.biSizeImage + (FILE_HEADER_LEN + INFO_HEADER_LEN);
        }

        void set_height(unsigned int h)
        {
            ihdt.biHeight = h;
            ihdt.biSizeImage = ihdt.biHeight*ihdt.biWidth*(ihdt.biBitCount/8);
            fhdt.bfSize = ihdt.biSizeImage + (FILE_HEADER_LEN + INFO_HEADER_LEN);
        }

        unsigned int get_width()
        {
            return (ihdt.biWidth);
        }

        unsigned int get_bytes_per_pixel()
        {
            return (ihdt.biBitCount/8);
        }

        unsigned int get_width_bytes()
        {
            return (ihdt.biWidth*(ihdt.biBitCount/8));
        }

        unsigned int get_height()
        {
            return(ihdt.biHeight);
        }

        void print_file_header()
        {
            cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
            cout << "Printing File Header Values: "<< bfName << endl;
            fhdt.print();
            cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
        }

        void print_info_header()
        {
            cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
            cout << "Printing Info Header Member Values: "<< bfName << endl;
            ihdt.print();
            cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;
        }

        void read_row(unsigned int row, char* t_buff, ifstream& in_file)
        {
            unsigned int tmp_offset;
            tmp_offset = (FILE_HEADER_LEN+INFO_HEADER_LEN);
            tmp_offset += ((get_height()-(row+1))*get_width_bytes());
            in_file.seekg(tmp_offset, ios::beg);
            in_file.read(t_buff,get_width_bytes());
        }

        void write_row(unsigned int row, char* t_buff, ofstream& out_file)
        {
            unsigned int tmp_offset;
            tmp_offset = (FILE_HEADER_LEN+INFO_HEADER_LEN);
            tmp_offset += ((get_height()-(row+1))*get_width_bytes());
            out_file.seekp(tmp_offset, ios::beg);
            out_file.write(t_buff,get_width_bytes());
        }
    
}; //bmp_data

#endif  // _BMP_HEADER_H_
