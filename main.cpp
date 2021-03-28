#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <termios.h>
#include <unistd.h>

class busy {
	public:
		busy() {}

		void show(int ms) {
			std::thread td = std::thread( [this] { interval(); } );
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
			terminate_ = true;
			td.join();
		}

	private:
		void interval() {
			terminate_ = false;
			std::vector < std::string > spinner {
				"▎   ",
				"▍   ",
				"▌   ",
				"▋   ",
				"▉   ",
				"▉▎  ",
				"▉▍  ",
				"▉▌  ",
				"▉▋  ",
				"▉▉  ",
				"▉▉▎ ",
				"▉▉▍ ",
				"▉▉▌ ",
				"▉▉▋ ",
				"▉▉▉ ",
				"▉▉▉▎",
				"▉▉▉▍",
				"▉▉▉▌",
				"▉▉▉▋",
				"▉▉▉▉",
				"▉▉▉▉",
				"▉▉▉▊",
				"▉▉▉▋",
				"▉▉▉▌",
				"▉▉▉▍",
				"▉▉▉▎",
				"▉▉▉ ",
				"▉▉▊ ",
				"▉▉▋ ",
				"▉▉▌ ",
				"▉▉▍ ",
				"▉▉▎ ",
				"▉▉  ",
				"▉▊  ",
				"▉▋  ",
				"▉▌  ",
				"▉▍  ",
				"▉▎  ",
				"▉   ",
				"▊   ",
				"▋   ",
				"▌   ",
				"▍   ",
				"▎   "
			};
			int i = 0;
			std::cout << "\033[40m"; //set the colour
			std::cout << "\033[?25l"; std::cout.flush(); //hide the cursor, for some reason \033 works when \01b does not
			
			termios oldt;
			tcgetattr(STDIN_FILENO, &oldt);
			termios newt = oldt;
			newt.c_lflag &= ~ECHO;
			tcsetattr(STDIN_FILENO, TCSANOW, &newt);
			
			while(!terminate_) { //keep looping
				std::cout << "\033[s"; //save the cursor position
				std::cout << "\033[1;33m" << spinner[i] << "\033[30m"; std::cout.flush(); //render the animation
				i >= spinner.size()-1 ? i = 0 : i++; //setup next animation
			
				std::this_thread::sleep_for(std::chrono::milliseconds(20)); //animation pause
				std::cout << "\033[" << spinner[i].length() << "D"; std::cout.flush(); //clear the animation
				std::cout << "\033[u"; //restore the cursor to the last saved position
			}
			std::cout << "\033[0m" << "\033[?25h"; std::cout.flush(); //reset colours and show the cursor			
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		}

		bool terminate_;
};

int main() {
	busy busy;
	
	busy.show(5000);
	std::cout << "got here1"; std::cout.flush();	
}
