#Knaves NES Emulator
The Knaves NES Emulator is an emulator for the Nintendo Entertainment System written in C++. It was created by [Nikolai Savas](http://savas.ca), [Joe Crozier](http://joecrozier.ca), and [Sam Nalwa](http://nalwa.ca/).

##Development
Knaves NES was developed using Visual Studio Community 2015 as well as SDL 2.0.3 (x64). Pre-compiled libraries and DLLs for SDL 2.0.3 that are compatible with Visual Studio Community 2015 are included in the /lib folder, but of course it's always possible to compile it yourself. It should be noted that most pre-compiled versions do not work with Visual Studio 2015 because of changes in the printf() function. 

##How to Run
Using command prompt, navigate to `knaves-nes/x64/Release`. To run the CPU emulator, use:

    ./knaves-nes.exe bubble_sort.nes
 
This would run the file "bubble_sort.nes" in verbose mode. To run it in performance mode (to test CPU speed), type:
 

    ./knaves-nes.exe bubble_sort.nes -perf

Some files have been included in this directory for testing, including "bubble_sort.nes", "countdown_loop.nes", "countdown_loop_16bit.nes".

During runtime, memory will be logged to a file named Memory - \*.log, where \* refers to the time of writing. Currently memory is printed at the start and end, as well as an intermediary print on the NOP instruction. If you're using this CPU emulator to test 6502 assembly code, adding NOPs as a method of debugging will allow the memory at that point to be viewed. 

Please note that running in verbose mode (logging) will incur large performance costs as it must write to a file during runtime. 

##Feature Set
Currently, the Knaves NES only emulates the 6502 MOS CPU of the NES. The PPU for the emulator is currently in development.