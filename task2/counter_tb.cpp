#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

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

    //initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 1;

    // run simulation for many clock cycles
    for(i=0; i<300; i++){

        //dump variables into VCD file and toggle clock
        for( clk=0; clk<2; clk++){  //forces model to evaluate on both edges of clock
            tfp->dump(2*i+clk); // unit in pico secs!
            top->clk = !top->clk;
            top->eval();
        }
        top->rst = (i<2) | (i==15);
        if(top->count == 9){
            top->en = 0;
            if(pause<3){
                pause++;
            } else{
                top->en = 1;
                pause = 0;
            }
        }
        if(Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}