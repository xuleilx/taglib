#include <iostream>
#include <iomanip>
#include <stdio.h>
#include<fstream>
#include <libgen.h>

#include <fileref.h>
#include <tpropertymap.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2) {
        std::cout<<argv[0]<<" filePath"<<std::endl;
        return 0;
    }
    std::string path = argv[1];

    //1.获取不带路径的文件名
    std::string::size_type iPos = path.find_last_of('\\') + 1;
    std::string filename = path.substr(iPos, path.length() - iPos);
    cout << "Input: "<<filename << endl;

    //2.获取不带后缀的文件名
    string name = filename.substr(0, filename.rfind("."));
    cout <<"Output: "<< name + ".jpg"<< endl;

    TagLib::MPEG::File f(path.data());

    if(f.hasID3v2Tag()) {

        TagLib::ID3v2::FrameList frame_list = f.ID3v2Tag()->frameListMap()["APIC"];

        if (frame_list.isEmpty())
        {
            std::cout<<"There seem to be no picture frames (APIC) in this audio file."<<std::endl;
            return false;
        }
        TagLib::ID3v2::AttachedPictureFrame* pic_frame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frame_list.front());
        if (pic_frame == nullptr)
        {
            std::cout<<"There seem to be no attached picture frames in this audio file."<<std::endl;
            return false;
        }
        std::cout<< "MimeType:"<<pic_frame->mimeType() <<std::endl;
        std::ofstream  dest_image(name +".jpg");//此处类型可能不对
        if (!dest_image)
        {
            std::cout<<"open failed!"<<std::endl;
            return false;
        }
        dest_image.write(pic_frame->picture().data(), pic_frame->picture().size());
        dest_image.flush();
        dest_image.close();
    }

  return 0;
}
