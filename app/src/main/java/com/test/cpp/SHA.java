package com.test.cpp;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.util.Base64;
import java.security.MessageDigest;

public class SHA {
    public static String get(Context context){
        try{
        PackageManager pm= context.getPackageManager();
        String pn = context.getPackageName();
        int signature = pm.GET_SIGNATURES;
        PackageInfo info = pm.getPackageInfo(pn, signature);
        Signature[] signatures = info.signatures;
            MessageDigest md = MessageDigest.getInstance("SHA");
            Signature sg = signatures[0];
            md.update(sg.toByteArray());
            return Base64.encodeToString(md.digest(), 0);
        } catch (Exception e) {
            return e.toString();
        }
    }
}
