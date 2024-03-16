package com.test.cpp;

import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import com.test.cpp.databinding.ActivityMainBinding;
import java.io.File;
import java.nio.file.Path;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity {
    ActivityMainBinding binding;
    TextView tv;
    LinearLayout base;

    static {
        try {
            System.loadLibrary("soko-lib");
            Log.d("LibraryLoad", "Library loaded successfully");
        } catch (UnsatisfiedLinkError e) {
            Log.e("LibraryLoad", "Failed to load library: " + e.getMessage());
        }
    }

   // public native void accessDataAndRepair();

  //  public native void testMutiply(int a);

  //  public native void changeTextViewBackground();

  //  public native void setTVColor();
    
  //  public native void showToast();
    
 //   public native void showDialog();
    
    // public native String getSignature();
    
    public native void checkSignature();
    
    //public native int getPath();
    
    //public native long getDex();
    
    //public native void initialize();
    
    //public native void addTvView();
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        
        tv = binding.tv;
        base = binding.base;
        
        //tv.setText(SHA.get(this));
        tv.setTextIsSelectable(true);
        tv.setText(SHA.get(this));
        
        //String apkPath = ApkPathGetter.getApkPath(this);
        
        
        //initialize();
        
        //tv.append(getSignature());
        
        //showToast();
        //showDialog();
        
      //  checkSignature();
        /*

        tv = binding.tv;
        tv.append(count + "\n");
        tv.append(name + "\n");
        accessDataAndRepair();
        tv.append(count + "\n");
        tv.append(name + "\n");
        testMutiply(30);
        tv.append(count + "\n");

        Timer timer = new Timer();
        TimerTask task =
                new TimerTask() {
                    @Override
                    public void run() {
                        runOnUiThread(
                                new Runnable() {
                                    @Override
                                    public void run() {
                                        setTVColor();
                                    }
                                });
                    }
                };

        timer.scheduleAtFixedRate(task, 0, 3000);
        
        */
    }
}
