#include <iostream>
#include <fstream>
#include <stdlib.h>
//#include <unistd.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <string>
#include <vector>
//#include <openssl/evp.h>
//#include <openssl/md5.h>
#define NUM_THREADS	4

struct thread_data
{
    int	thread_id;
    std::string filename;
};

thread_data thread_data_array[NUM_THREADS];
std::vector<std::string> passwords;
std::string CURRENT_FILENAME;

static std::mutex mut, mut2;

void calc_MD5(unsigned char* input, unsigned char* output) {
    /*
        unsigned int length;
        while (input[length] != '\0')
                length++;
        MD5_CTX ctx;
        MD5_Init(&ctx);
        MD5_Update(&ctx, input, length);
        MD5_Final(output, &ctx);
        */
}

bool Check_filename(const std::string& OFN)
{
    if (OFN == CURRENT_FILENAME)
        return false;
    else
    {
        return true;
    }
}

void Encrypt(std::string en)
{
    /*
    unsigned char* writable = new unsigned char[en.size() + 1];
    unsigned char* output = NULL;
    std::copy(en.begin(), en.end(), writable);
    writable[en.size()] = '\0';

    calc_MD5(writable, output);
    */
    mut.lock();
    passwords.emplace_back(en);
    mut.unlock();
}

void Producent(int t)//thread_data& threadarg)
{
    //variables
    std::cout << "Thread Created, ID: " << t << std::endl;
    thread_data& threadarg = thread_data_array[t];
    std::vector<std::string> passff;
    std::string from_file = "";
    std::string OFN = threadarg.filename;
    std::string number = "";
    int k = 0;
    std::ifstream file;
    //read from file 
    try {
        file.open(OFN);
        if (!file.good()) throw std::runtime_error("INCORRECT_FILE_NAME");
    }
    catch (std::exception& e)
    {
        std::cout << "An Exception has occured: " << e.what() <<std::endl;
    }
    if (file.good())
    {
        while (std::getline(file, from_file))
        {
            passff.emplace_back(from_file);
        }
    }
    //ta czesc odpowiada za to, zeby hasla mialy poprawny format
    if (threadarg.thread_id == 0)
    {
        for (unsigned int i = 0; i < passff.size(); i++)
        {
            for (unsigned int j = 0; j < passff[i].length();  j++)
                if (passff[i][j] >= 65 && passff[i][j] <= 90) { passff[i][j] += 32; }
        }
    }
    if (threadarg.thread_id == 1)
    {
        for (unsigned int i = 0; i < passff.size(); i++)
        {
            if (passff[i][0] >= 97 && passff[i][0] <= 122) { passff[i][0] -= 32; }
        }
    }
    else if (threadarg.thread_id == 2)
    {
        for (unsigned int i = 0; i < passff.size(); i++)
        {
            for (unsigned int j = 0; j < passff[i].length(); j++)
                if (passff[i][j] >= 97 && passff[i][j] <= 122) { passff[i][j] -= 32; }
        }
    }
    else {}
    while (true) {
        //sprawdzam czy nazwa pliku sie zmienila
        if (CURRENT_FILENAME == "exit") break;
        else if (CURRENT_FILENAME == "cont" || CURRENT_FILENAME == "print") {}
        else if (Check_filename(OFN))
        {
            if (CURRENT_FILENAME != "cont" || CURRENT_FILENAME != "print")
            //czyszcze poprzedni vector hasel
            std::cout << "Changing File, ThreadID: " << t << std::endl;
            passff.clear();
            file.close();
            try {
                std::ifstream file(CURRENT_FILENAME);
                if (!file.good()) throw std::runtime_error("INCORRECT_FILE_NAME");
            }
            catch (std::exception& e) 
                {
                std::cerr << "An Exception has occured: " << e.what() << std::endl;
                std::cerr << "File name: " << CURRENT_FILENAME << std::endl;
                }

            while (std::getline(file, from_file))
            {
                passff.emplace_back(from_file);
            }
            //ta czesc odpowiada za to, zeby hasla mialy poprawny format
            if (threadarg.thread_id == 0)
            {
                for (unsigned int i = 0; i < passff.size(); i++)
                {
                    for (unsigned int j = 0; j < passff[i].length(); j++)
                        if (passff[i][j] >= 65 && passff[i][j] <= 90)
                            passff[i][j] += 32;
                }
            }
            if (threadarg.thread_id == 1)
            {
                for (unsigned int i = 0; i < passff.size(); i++)
                {
                    if (passff[i][0] >= 97 && passff[i][0] <= 122)
                        passff[i][0] -= 32;
                }
            }
            else if (threadarg.thread_id == 2)
            {
                for (unsigned int i = 0; i < passff.size(); i++)
                {
                    for (unsigned int j = 0; j < passff[i].length(); j++)
                        if (passff[i][j] >= 97 && passff[i][j] <= 122)
                            passff[i][j] -= 32;
                }
            }
            else {}
            OFN == CURRENT_FILENAME;
            std::cout << "File changed, ThreadID: " << t << std::endl;
        }
        else {}
        //encrypt
        for (unsigned int i = 0; i < passff.size(); i++)
        {
            number = std::to_string(k);
            from_file = number + passff[i] + number;
            Encrypt(from_file);
        }
        k++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    file.close();
    std::cout << "Thread Closing, ID: " << t << std::endl;
    //pthread_exit(NULL);
}

void Decrypt(int t)//thread_data& threadarg)
{
    thread_data& threadarg = thread_data_array[t];

    //pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    int t;
    std::string input = "";
    std::thread Prod[3];
    std::cout << "Podaj nazwe pliku: ";
    std::cin >> input;
    std::cout << "\nnazwa pobrana\n";
    CURRENT_FILENAME = input;

    for (t = 0; t < NUM_THREADS-1; t++)
    {
        thread_data_array[t].thread_id = t;
        thread_data_array[t].filename = input;

        printf("Creating thread %d\n", t);

        Prod[t] = std::thread(Producent, t);
    }

    while (true)
    {
        input = "";
        std::cout << "Command: ";
        std::cin >> input;
        
        if ("cont" == input || "print" == input) {}
        if (input != "")
        {
            mut2.lock();
            CURRENT_FILENAME = input;
            mut2.unlock();
        }
        if ("exit" == input) break;
        mut.lock();
        for (std::string el : passwords)
        {
            std::cout << el << std::endl;;
        }
        mut.unlock();
    }
    for (int i = 0; i < 4; i++)
    {
        if (Prod[i].joinable())
        {
           Prod[i].join();
        }
    }
    std::cout<< "Exiting";
    EXIT_SUCCESS;
    //pthread_exit(NULL);
}