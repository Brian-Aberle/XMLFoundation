package com.example.myapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.example.myapp.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    long _handle = CreateAppHandle();

    // Used to load the 'myapp' library on application startup.
    static {
        System.loadLibrary("myapp");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());


        binding.btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // if the [section] and "Entry=" does not exist it will be created otherwise updated
                // this will store "The Value" in the application configuration profile shared by the C++ and Java layers
                SetAppValue(_handle,"Section","Entry","The Value");

                tv.setText(GetAppValue(_handle,"Section","Entry"));
            }
        });



    }




    // see the corresponding interfaces in native-lib.cpp and the implementation in XMLFoundationApp.cpp for the following methods
    public native String stringFromJNI();
    public native long CreateAppHandle();
    public native void DeleteAppHandle(long handle);
    public native int SetAppValue(long handle, String sSection, String sEntry, String sValue);
    public native String GetAppValue(long handle,String sSection, String sEntry);

}