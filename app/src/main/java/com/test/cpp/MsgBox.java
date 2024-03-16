package com.test.cpp;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;

public class MsgBox {
    AlertDialog alertDialog;

    public MsgBox(Activity activity) {
        alertDialog = new AlertDialog.Builder(activity).create();

        alertDialog.setButton(
                AlertDialog.BUTTON_POSITIVE,
                "OK",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int arg1) {
                        dialog.dismiss();
                    }
                });
    }
    
    public AlertDialog getAlertDialog(){
        return alertDialog;
    }
}
