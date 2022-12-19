package com.kami;

public class test2 {

    public test2() {
        try{
            Runtime.getRuntime().exec("/Applications/Calculator.app/Contents/MacOS/Calculator");
        }catch(Exception e){
            e.printStackTrace();
        }
    }
}

