package de.kai_morich.simple_usb_terminal;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.media.AudioAttributes;
import android.media.SoundPool;
import android.os.Bundle;
import android.os.IBinder;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.method.ScrollingMovementMethod;
import android.text.style.ForegroundColorSpan;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.driver.UsbSerialProber;

public class TerminalFragment extends Fragment implements ServiceConnection, SerialListener {

    private enum Connected { False, Pending, True }

    public static final String INTENT_ACTION_GRANT_USB = BuildConfig.APPLICATION_ID + ".GRANT_USB";

    private int deviceId, portNum, baudRate;
    private String newline = "\r\n";

    private TextView receiveText;

    private SerialSocket socket;
    private SerialService service;
    private boolean initialStart = true;
    private Connected connected = Connected.False;
    private BroadcastReceiver broadcastReceiver;

    SoundPool soundPool;
    int sound1;
    int sound2;
    int sound3;
    int sound4;
    int sound5;
    int sound6;
    int sound7;
    int sound8;
    int sound9;
    int sound10;
    int sound11;
    int sound12;
    int sound13;
    int sound14;
    int sound15;
    int sound16;
    int sound17;
    int sound18;
    int sound19;
    int sound20;


    Context c;

    public TerminalFragment() {
        broadcastReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                if(intent.getAction().equals(INTENT_ACTION_GRANT_USB)) {
                    Boolean granted = intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false);
                    connect(granted);
                }
            }
        };
    }

    /*
     * Lifecycle
     */
    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
        setRetainInstance(true);
        deviceId = getArguments().getInt("device");
        portNum = getArguments().getInt("port");
        baudRate = getArguments().getInt("baud");

        soundPool = new SoundPool.Builder()
                .setMaxStreams(20)
                .setAudioAttributes(new AudioAttributes.Builder()
                        .setUsage(AudioAttributes.USAGE_MEDIA)
                        .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                        .build()
                )
                .build();
        sound1 = soundPool.load(getActivity().getApplicationContext(),R.raw.crashone,1);
        sound2 = soundPool.load(getActivity().getApplicationContext(),R.raw.crashtwo,1);
        sound3 = soundPool.load(getActivity().getApplicationContext(),R.raw.hhopenone,1);
        sound4 = soundPool.load(getActivity().getApplicationContext(),R.raw.hhopentwo,1);
        sound5 = soundPool.load(getActivity().getApplicationContext(),R.raw.hihatone,1);
        sound6 = soundPool.load(getActivity().getApplicationContext(),R.raw.hihattwo,1);
        sound7 = soundPool.load(getActivity().getApplicationContext(),R.raw.kickelectric,1);
        sound8 = soundPool.load(getActivity().getApplicationContext(),R.raw.kickone,1);
        sound9 = soundPool.load(getActivity().getApplicationContext(),R.raw.kicktwo,1);
        sound10 = soundPool.load(getActivity().getApplicationContext(),R.raw.snareone,1);
        sound11 = soundPool.load(getActivity().getApplicationContext(),R.raw.snaretwo,1);
        sound12 = soundPool.load(getActivity().getApplicationContext(),R.raw.taiko,1);
        sound13 = soundPool.load(getActivity().getApplicationContext(),R.raw.taikostick,1);
        sound14 = soundPool.load(getActivity().getApplicationContext(),R.raw.tomone,1);
        sound15 = soundPool.load(getActivity().getApplicationContext(),R.raw.tomtwo,1);
        sound16 = soundPool.load(getActivity().getApplicationContext(),R.raw.tomthree,1);
        sound17 = soundPool.load(getActivity().getApplicationContext(),R.raw.tap,1);
        sound18 = soundPool.load(getActivity().getApplicationContext(),R.raw.slap,1);
        sound19 = soundPool.load(getActivity().getApplicationContext(),R.raw.mariocoin,1);
        sound20 = soundPool.load(getActivity().getApplicationContext(),R.raw.mariojump,1);
    }

    @Override
    public void onDestroy() {
        if (connected != Connected.False)
            disconnect();
        getActivity().stopService(new Intent(getActivity(), SerialService.class));
        super.onDestroy();
    }

    @Override
    public void onStart() {
        super.onStart();
        if(service != null)
            service.attach(this);
        else
            getActivity().startService(new Intent(getActivity(), SerialService.class)); // prevents service destroy on unbind from recreated activity caused by orientation change
    }

    @Override
    public void onStop() {
        if(service != null && !getActivity().isChangingConfigurations())
            service.detach();
        super.onStop();
    }

    @SuppressWarnings("deprecation") // onAttach(context) was added with API 23. onAttach(activity) works for all API versions
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        getActivity().bindService(new Intent(getActivity(), SerialService.class), this, Context.BIND_AUTO_CREATE);
    }

    @Override
    public void onDetach() {
        try { getActivity().unbindService(this); } catch(Exception ignored) {}
        super.onDetach();
    }

    @Override
    public void onResume() {
        super.onResume();
        getActivity().registerReceiver(broadcastReceiver, new IntentFilter(INTENT_ACTION_GRANT_USB));
        if(initialStart && service !=null) {
            initialStart = false;
            getActivity().runOnUiThread(this::connect);
        }
    }

    @Override
    public void onPause() {
        getActivity().unregisterReceiver(broadcastReceiver);
        super.onPause();
    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder binder) {
        service = ((SerialService.SerialBinder) binder).getService();
        if(initialStart && isResumed()) {
            initialStart = false;
            getActivity().runOnUiThread(this::connect);
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        service = null;
    }

    /*
     * UI
     */
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_terminal, container, false);
        receiveText = view.findViewById(R.id.receive_text);                          // TextView performance decreases with number of spans
        receiveText.setTextColor(getResources().getColor(R.color.colorRecieveText)); // set as default color to reduce number of spans
        receiveText.setMovementMethod(ScrollingMovementMethod.getInstance());
        TextView sendText = view.findViewById(R.id.send_text);
        View sendBtn = view.findViewById(R.id.send_btn);
        sendBtn.setOnClickListener(v -> send(sendText.getText().toString()));
        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        inflater.inflate(R.menu.menu_terminal, menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.clear) {
            receiveText.setText("");
            return true;
        } else if (id ==R.id.newline) {
            String[] newlineNames = getResources().getStringArray(R.array.newline_names);
            String[] newlineValues = getResources().getStringArray(R.array.newline_values);
            int pos = java.util.Arrays.asList(newlineValues).indexOf(newline);
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setTitle("Newline");
            builder.setSingleChoiceItems(newlineNames, pos, (dialog, item1) -> {
                newline = newlineValues[item1];
                dialog.dismiss();
            });
            builder.create().show();
            return true;
        } else {
            return super.onOptionsItemSelected(item);
        }
    }

    /*
     * Serial + UI
     */
    private void connect() {
        connect(null);
    }

    private void connect(Boolean permissionGranted) {
        UsbDevice device = null;
        UsbManager usbManager = (UsbManager) getActivity().getSystemService(Context.USB_SERVICE);
        for(UsbDevice v : usbManager.getDeviceList().values())
            if(v.getDeviceId() == deviceId)
                device = v;
        if(device == null) {
            status("connection failed: device not found");
            return;
        }
        UsbSerialDriver driver = UsbSerialProber.getDefaultProber().probeDevice(device);
        if(driver == null) {
            driver = CustomProber.getCustomProber().probeDevice(device);
        }
        if(driver == null) {
            status("connection failed: no driver for device");
            return;
        }
        if(driver.getPorts().size() < portNum) {
            status("connection failed: not enough ports at device");
            return;
        }
        UsbSerialPort usbSerialPort = driver.getPorts().get(portNum);
        UsbDeviceConnection usbConnection = usbManager.openDevice(driver.getDevice());
        if(usbConnection == null && permissionGranted == null) {
            if (!usbManager.hasPermission(driver.getDevice())) {
                PendingIntent usbPermissionIntent = PendingIntent.getBroadcast(getActivity(), 0, new Intent(INTENT_ACTION_GRANT_USB), 0);
                usbManager.requestPermission(driver.getDevice(), usbPermissionIntent);
                return;
            }
        }
        if(usbConnection == null) {
            if (!usbManager.hasPermission(driver.getDevice()))
                status("connection failed: permission denied");
            else
                status("connection failed: open failed");
            return;
        }

        connected = Connected.Pending;
        try {
            socket = new SerialSocket();
            service.connect(this, "Connected");
            socket.connect(getContext(), service, usbConnection, usbSerialPort, baudRate);
            // usb connect is not asynchronous. connect-success and connect-error are returned immediately from socket.connect
            // for consistency to bluetooth/bluetooth-LE app use same SerialListener and SerialService classes
            onSerialConnect();
        } catch (Exception e) {
            onSerialConnectError(e);
        }
    }

    private void disconnect() {
        connected = Connected.False;
        service.disconnect();
        socket.disconnect();
        socket = null;
    }

    private void send(String str) {
        if(connected != Connected.True) {
            Toast.makeText(getActivity(), "not connected", Toast.LENGTH_SHORT).show();
            return;
        }
        try {
            SpannableStringBuilder spn = new SpannableStringBuilder(str+'\n');
            spn.setSpan(new ForegroundColorSpan(getResources().getColor(R.color.colorSendText)), 0, spn.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
            receiveText.append(spn);
            byte[] data = (str + newline).getBytes();
            socket.write(data);
        } catch (Exception e) {
            onSerialIoError(e);
        }
    }

    private void receive(byte[] data) {
        String s = new String(data);
        receiveText.append(s);
//        if(s.equals("9")){
//            receiveText.append("---nine---");
//        }
//        else if(s.equals("18")){
//            receiveText.append("---dope!!!---");
//        }

//        if(s.contains("a"))soundPool.play(sound1, 1, 1, 1, 0, 1);
//        if(s.contains("b"))soundPool.play(sound2, 1, 1, 1, 0, 1);
//        if(s.contains("c"))soundPool.play(sound3, 1, 1, 1, 0, 1);
//        if(s.contains("d"))soundPool.play(sound4, 1, 1, 1, 0, 1);
//        if(s.contains("e"))soundPool.play(sound5, 1, 1, 1, 0, 1);
//        if(s.contains("f"))soundPool.play(sound6, 1, 1, 1, 0, 1);
//        if(s.contains("g"))soundPool.play(sound7, 1, 1, 1, 0, 1);
//        if(s.contains("h"))soundPool.play(sound8, 1, 1, 1, 0, 1);
//        if(s.contains("i"))soundPool.play(sound9, 1, 1, 1, 0, 1);
//        if(s.contains("j"))soundPool.play(sound10, 1, 1, 1, 0, 1);
//        if(s.contains("k"))soundPool.play(sound11, 1, 1, 1, 0, 1);
//        if(s.contains("l"))soundPool.play(sound12, 1, 1, 1, 0, 1);
//        if(s.contains("m"))soundPool.play(sound13, 1, 1, 1, 0, 1);
//        if(s.contains("n"))soundPool.play(sound14, 1, 1, 1, 0, 1);
//        if(s.contains("o"))soundPool.play(sound15, 1, 1, 1, 0, 1);
//        if(s.contains("p"))soundPool.play(sound16, 1, 1, 1, 0, 1);
//        if(s.contains("q"))soundPool.play(sound17, 1, 1, 1, 0, 1);
//        if(s.contains("r"))soundPool.play(sound18, 1, 1, 1, 0, 1);
//        if(s.contains("s"))soundPool.play(sound19, 1, 1, 1, 0, 1);
//        if(s.contains("t"))soundPool.play(sound20, 1, 1, 1, 0, 1);


        while(s.length() > 0) {

            switch (s.charAt(0)) {
                case 'a':
                    soundPool.play(sound1, 1, 1, 1, 0, 1);
                    s = s.replaceAll("a","");
                    break;
                case 'b':
                    soundPool.play(sound2, 1, 1, 1, 0, 1);
                    s = s.replaceAll("b","");
                    break;
                case 'c':
                    soundPool.play(sound3, 1, 1, 1, 0, 1);
                    s = s.replaceAll("c","");
                    break;
                case 'd':
                    soundPool.play(sound4, 1, 1, 1, 0, 1);
                    s = s.replaceAll("d","");
                    break;
                case 'e':
                    soundPool.play(sound5, 1, 1, 1, 0, 1);
                    s = s.replaceAll("e","");
                    break;
                case 'f':
                    soundPool.play(sound6, 1, 1, 1, 0, 1);
                    s = s.replaceAll("f","");
                    break;
                case 'g':
                    soundPool.play(sound7, 1, 1, 1, 0, 1);
                    s = s.replaceAll("g","");
                    break;
                case 'h':
                    soundPool.play(sound8, 1, 1, 1, 0, 1);
                    s = s.replaceAll("h","");
                    break;
                case 'i':
                    soundPool.play(sound9, 1, 1, 1, 0, 1);
                    s = s.replaceAll("i","");
                    break;
                case 'j':
                    soundPool.play(sound10, 1, 1, 1, 0, 1);
                    s = s.replaceAll("j","");
                    break;
                case 'k':
                    soundPool.play(sound11, 1, 1, 1, 0, 1);
                    s = s.replaceAll("k","");
                    break;
                case 'l':
                    soundPool.play(sound12, 1, 1, 1, 0, 1);
                    s = s.replaceAll("l","");
                    break;
                case 'm':
                    soundPool.play(sound13, 1, 1, 1, 0, 1);
                    s = s.replaceAll("m","");
                    break;
                case 'n':
                    soundPool.play(sound14, 1, 1, 1, 0, 1);
                    s = s.replaceAll("n","");
                    break;
                case 'o':
                    soundPool.play(sound15, 1, 1, 1, 0, 1);
                    s = s.replaceAll("o","");
                    break;
                case 'p':
                    soundPool.play(sound16, 1, 1, 1, 0, 1);
                    s = s.replaceAll("p","");
                    break;
                case 'q':
                    soundPool.play(sound17, 1, 1, 1, 0, 1);
                    s = s.replaceAll("q","");
                    break;
                case 'r':
                    soundPool.play(sound18, 1, 1, 1, 0, 1);
                    s = s.replaceAll("r","");
                    break;
                case 's':
                    soundPool.play(sound19, 1, 1, 1, 0, 1);
                    s = s.replaceAll("s","");
                    break;
                case 't':
                    soundPool.play(sound20, 1, 1, 1, 0, 1);
                    s = s.replaceAll("t","");
                    break;
                default:
                    break;


            }
        }
    }

    private void status(String str) {
        SpannableStringBuilder spn = new SpannableStringBuilder(str+'\n');
        spn.setSpan(new ForegroundColorSpan(getResources().getColor(R.color.colorStatusText)), 0, spn.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        receiveText.append(spn);
    }

    /*
     * SerialListener
     */
    @Override
    public void onSerialConnect() {
        status("connected");
        connected = Connected.True;
    }

    @Override
    public void onSerialConnectError(Exception e) {
        status("connection failed: " + e.getMessage());
        disconnect();
    }

    @Override
    public void onSerialRead(byte[] data) {
        receive(data);
    }

    @Override
    public void onSerialIoError(Exception e) {
        status("connection lost: " + e.getMessage());
        disconnect();
    }

}
