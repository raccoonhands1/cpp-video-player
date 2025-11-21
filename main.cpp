#include <chrono>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <unordered_map>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
  unordered_map<string, string> presetVideos;

  presetVideos["Bad Apple"] =
      "/home/raccoonhands/Programming/Code/BadApple/"
      "bad-apple-but-its-on-the-leetcode-calendar/BA.mp4";
  presetVideos[""] = "";

  string videoPath;

  const char DISPLAY_VALUES[10] = {'-', '_', '^', '=', '*',
                                   '[', '0', '$', '#', '@'};
  double NORMALIZATION_FACTOR = 1;
  int RESOLUTION_SCALE = 7;
  int SPACING = 1;

  if (argc == 1) {
    cout << "Use --help to get help. use --play to get that sheet played"
         << endl;
    return -1;
  }

  if (argc > 1) {
    // Parse all arguments
    for (int i = 1; i < argc; i++) {
      string arg = argv[i];

      if (arg == "--help") {
        cout << "Usage: ./main [OPTIONS] --play [VIDEO_PATH]" << endl;
        cout << "Options:" << endl;
        cout << "  --normalization [factor]  Set normalization factor "
                "(default: 1, range: 0-2)"
             << endl;
        cout << "  --res [scale]             Set resolution scale (default: 7, "
                "higher = lower res)"
             << endl;
        cout << "  --space [spaces]          Set character spacing (default: 1)"
             << endl;
        cout << "  --play [video_path]       Play specified video file" << endl;
        cout << "  --help                    Show this help message" << endl;
        cout << "\nVideos list: \n";
        for (auto it = presetVideos.begin(); it != presetVideos.end(); it++) {
          cout << it->first << " : " << it->second << endl;
        }
        return 0;
      } else if (arg == "--normalization" && i + 1 < argc) {
        NORMALIZATION_FACTOR = stod(argv[i + 1]);
        cout << "Normalization factor set to: " << NORMALIZATION_FACTOR << endl;
        i++; // Skip the next argument since it's the normalization value
      } else if (arg == "--res" && i + 1 < argc) {
        RESOLUTION_SCALE = stoi(argv[i + 1]);
        cout << "Resolution scale set to: " << RESOLUTION_SCALE << endl;
        i++; // Skip the next argument since it's the resolution value
      } else if (arg == "--space" && i + 1 < argc) {
        SPACING = stoi(argv[i + 1]);
        cout << "Character spacing set to: " << SPACING << endl;
        i++; // Skip the next argument since it's the spacing value
      } else if (arg == "--play" && i + 1 < argc) {
        string videoName = argv[i + 1];
        i++; // Skip the next argument since it's the video name

        // Check if video name exists in the map
        if (presetVideos.find(videoName) != presetVideos.end()) {
          videoPath = presetVideos[videoName];
        } else {
          // Assume videoName is a path if not found in the map
          videoPath = videoName;
        }
        // Don't break here - continue parsing other arguments
      }
    }

    // If videoPath is set, play the video
    if (!videoPath.empty()) {
      // Check if the file exists
      ifstream fileCheck(videoPath);
      if (fileCheck.good()) {
        cout << "Playing video at path: " << videoPath << endl;
        fileCheck.close();

        VideoCapture cap(videoPath);

        // Check if the video opened successfully
        if (!cap.isOpened()) {
          cout << "Error opening video stream or file" << endl;
          return -1;
        }

        Mat frame;
        const int X = RESOLUTION_SCALE, Y = RESOLUTION_SCALE;
        const int W = cap.get(CAP_PROP_FRAME_WIDTH);
        const int H = cap.get(CAP_PROP_FRAME_HEIGHT);

        while (cap.read(frame)) {
          if (frame.empty())
            break;

          Mat grayImage;
          cvtColor(frame, grayImage, COLOR_BGR2GRAY);

          string buffer;
          buffer.reserve((W / X) * (H / Y) * 2);

          uchar intensity;

          if (intensity > 150) {
            cout << "nihao world" << endl;
          }

          for (int j = 0; j < H; j += Y) {
            for (int i = 0; i < W; i += X) {
              intensity = grayImage.at<uchar>(j, i);

              int index =
                  min(9, int(intensity / (25.5 / NORMALIZATION_FACTOR)));
              buffer += DISPLAY_VALUES[index];
              for (int s = 0; s < SPACING; s++) {
                buffer += " ";
              }
            }
            buffer += '\n';
          }
          cout << buffer;
          this_thread::sleep_for(32ms);
        }

        cap.release();
        destroyAllWindows();
      } else {
        cout << "Error: File does not exist or cannot be accessed." << endl;
        fileCheck.close();
        return -1;
      }
    } else {
      cout << "Error: No video specified for playback." << endl;
      return -1;
    }
  } else {
    cout << "Error: No arguments provided. Use --help to see available options."
         << endl;
    return -1;
  }

  return 0;
}
