#include <Magick++.h>
#include <filesystem>
#include <iostream>
#include <optional>

void ensure_next_arg(int argc, int current_index) {
    if (current_index + 1 >= argc) {
        throw std::runtime_error("Expected argument after option");
    }
}

void help()
{
    std::cout << 
        "Usage: cvtwebp [options] <input_image> [<output_image>]\n"
        "Options:\n"
        "  --lossless          Use lossless compression (default is lossy)\n"
        "  --quality <0-100>   Set quality for lossy compression (default is 90)\n"
        "  --scale <factor>    Scale the image by the given factor (can be in real numbers of percent)\n"
        "  -h, --help          Show this help message\n"
    << std::flush;
}

int main(int argc, char** argv) {
    std::optional<std::string> input_path;
    std::optional<std::string> output_path;
    int quality = 90;
    bool lossless = false;
    int arg_index = 1;
    float scale = 1.0f;
    while(arg_index < argc) {
        std::string_view arg = argv[arg_index];
        if(arg == "--lossless") {
            lossless = true;
            arg_index++;
        }
        else if(arg == "--quality") {
            ensure_next_arg(argc, arg_index);
            quality = std::stoi(argv[arg_index + 1]);
            arg_index += 2;
        }
        else if(arg == "--scale") {
            ensure_next_arg(argc, arg_index);
            std::string_view scale_arg = argv[arg_index + 1];
            if(scale_arg.back() == '%') {
                scale_arg.remove_suffix(1);
                scale = std::stof(std::string(scale_arg)) / 100.0f;
                arg_index += 2;
                continue;
            }
            scale = std::stof(argv[arg_index + 1]);
            arg_index += 2;
        }
        else if(arg == "-h" || arg == "--help") {
            help();
            return 0;
        }
        else if(arg.find("-") == 0) {
            std::cerr << "Error: Unknown option: " << arg << "\n";
            help();
            return 1;
        }
        else {
            if(!input_path)
                input_path = arg;
            else if(!output_path)
                output_path = arg;
            else {
                std::cerr << "Error: Unexpected argument: " << arg << "\n";
                help();
                return 1;
            }
            arg_index++;
        }
    }
    if(!input_path) {
        std::cerr << "Error: No input image specified.\n";
        help();
        return 1;
    }


    Magick::InitializeMagick(*argv);

    try {
        std::filesystem::path inputPath = *input_path;
        std::filesystem::path outputPath;
        if(output_path) {
            outputPath = *output_path;
            if(outputPath.extension() != ".webp") {
                std::cerr << "Error: Output file must have .webp extension.\n";
                return 1;
            }
        }
        else {
            outputPath = inputPath;
            outputPath.replace_extension(".webp");
        }

        Magick::Image image;
        image.read(inputPath.string());

        // Optional quality setting (0–100)
        if(lossless) {
            image.attribute("webp:lossless", "true");
        }
        else {
            image.quality(quality);
        }
        
        if(scale != 1.0f) {
            int newWidth = static_cast<int>(image.columns() * scale);
            int newHeight = static_cast<int>(image.rows() * scale);
            image.resize(Magick::Geometry(newWidth, newHeight));
        }

        image.strip();
        image.write(outputPath.generic_string());
    }
    catch (const Magick::Exception& e) {
        std::cerr << "ImageMagick error: " << e.what() << "\n";
        return 2;
    }

    return 0;
}

