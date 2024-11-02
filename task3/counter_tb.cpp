#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){
    int i;
    int clk;
    int pause = 0;

    Verilated::commandArgs(argc, argv);
    //init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open("counter.vcd"); // dumping waveform data to counter.vcd

    // init Vbuddy
    if(vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");   // port path is in vbuddy.cfg
    vbdSetMode(1);

    //initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 0;

    // run simulation for many clock cycles
    i=0;
    while(i<300){

        //;change input stimuli
        top->rst = (i<2) | (i==15);
        top->ld = vbdFlag();

        //dump variables into VCD file and toggle clock
        if(top->ld){
            for( clk=0; clk<2; clk++){  //forces model to evaluate on both edges of clock
            tfp->dump(2*i+clk); // unit in pico secs!
            top->clk = !top->clk;
            top->eval();
            }
            i++;
        }
        
        //if(top->ld) top->count = vbdValue();  // setting count to preset value

        // ++++ Send count value to Vbuddy
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        vbdCycle(i+1);
        // ---- end of Vbuddy output section

        // Testing plot function
        //vbdPlot(int(top->count), 0, 255);
        //vbdCycle(i+1);

        
        if(Verilated::gotFinish()) exit(0);

    }

    vbdClose();     // ++++
    tfp->close();
    exit(0);
}