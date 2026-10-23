#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

// FFmpeg লাইব্রেরি বা সিস্টেম কমান্ড ব্যবহার করে ব্যাচ প্রসেসিং করার ফাংশন
void processVideos(const std::string& inputDir, const std::string& outputDir) {
    // আউটপুট ফোল্ডার চেক করা এবং না থাকলে তৈরি করা
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    int count = 0;
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            // শুধুমাত্র ভিডিও ফাইলগুলো সিলেক্ট করা
            if (ext == ".mp4" || ext == ".mkv" || ext == ".mov" || ext == ".avi") {
                if (count >= 10) break; // একবারে সর্বোচ্চ ১০টি ভিডিওর জন্য
                count++;

                std::string inputPath = entry.path().string();
                std::string filename = entry.path().filename().string();
                std::string outputPath = outputDir + "/cpp_edited_" + filename;

                std::cout << "\n[" << count << "/10] প্রসেস হচ্ছে: " << filename << " ..." << std::endl;

                // FFmpeg কমান্ড তৈরি (C++ এর system() ফাংশন দিয়ে এক্সিকিউট করার জন্য)
                // -vf: ব্রাইটনেস ০.০৮ বাড়ানো এবং ভিডিও স্পিড ১.১x করা
                // -af: নয়েজ রিমুভ (afftdn) এবং অডিও স্পিড ১.১x করা
                std::string command = "ffmpeg -y -i \"" + inputPath + 
                                      "\" -vf \"eq=brightness=0.08,setpts=1/1.1*PTS\"" +
                                      " -af \"afftdn=nf=-25,atempo=1.1\"" +
                                      " -c:v libx264 -crf 23 -c:a aac -b:a 128k -ar 44100 -ac 2 \"" + 
                                      outputPath + "\"";

                // কমান্ড টার্মিনালে রান করা
                int result = std::system(command.c_str());
                if (result == 0) {
                    std::cout << "✅ সফলভাবে সম্পন্ন! সেভ হয়েছে: " << outputPath << std::endl;
                } else {
                    std::cout << "❌ ফাইল প্রসেস করার সময় ত্রুটি হয়েছে: " << filename << std::endl;
                }
            }
        }
    }
}

int main() {
    std::string inputFolder = "./input_videos";
    std::string outputFolder = "./output_videos";

    std::cout << "C++ এবং FFmpeg দিয়ে ব্যাচ ভিডিও এডিটিং শুরু হচ্ছে..." << std::endl;
    processVideos(inputFolder, outputFolder);
    std::cout << "\n🎉 সব ভিডিওর এডিটিং সফলভাবে শেষ হয়েছে!" << std::endl;

    return 0;
}
