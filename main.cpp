#include <iostream>
#include <fstream>

#include "processor.hpp"
#include "defines.hpp"


int main(int argc, char** argv)
{
    std::cout << "16 Bit Processor Demo" << "\n";

    if (argc < 2)
    {
        std::cout << "Expected Disk File\n";
        return -1;
    }

    Processor processor;

    std::ifstream in(argv[1], std::ios::binary);

    if (!in)
    {
        std::cout << "Unable to read file '" << argv[1] << "'\n";
        return -1;
    }

    in.seekg (0, in.end);
    int length = in.tellg();
    in.seekg (0, in.beg);

    if (length >= RAM_SIZE)
    {
        std::cout << "Disk File Larger than RAM Size (" << RAM_SIZE << "B)\n";
        return -1;
    }

    in.read((char*)processor.RAM, length);

    std::cout << "Loaded " << length << " bytes\n";

    processor.display_state();
    processor.dump_ram_segment(0x0000, 0x0020);

    while (processor.RAM[0xFFFF] == 0)
    {
        //printf("\n\n");
        if (processor.RAM[0x7FFF] != 0)
        {
            processor.RAM[0x7FFF] = 0;

            printf("\033[1;1H");
            printf("\033[0J");
            processor.render_display();
        }
        

        processor.execute_instruction();
        
        /*
        processor.display_state();
        processor.dump_ram_segment(processor.program_counter, 0x0040);
        processor.dump_ram_segment(0xFE0, 0x0040);
        processor.dump_ram_segment(0x7000, 0x10);
        processor.dump_ram_segment(0x7FFF, 0x20);

        char c;
        std::cin >> c;*/
    }

    processor.dump_ram_segment(0x0000, 0x0100);
    processor.dump_ram_segment(0x7FFF, 0x0200);

    return 0;
}
