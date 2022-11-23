package me.alexbakker.cve_2019_9465;

import android.content.DialogInterface;
import android.hardware.biometrics.BiometricPrompt;
import android.os.CancellationSignal;
import android.os.Handler;
import android.security.keystore.KeyGenParameterSpec;
import android.security.keystore.KeyProperties;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.KeyStore;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.Collections;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.GCMParameterSpec;

public class MainActivity extends AppCompatActivity {
    private int i = 0;
    private SecretKey _key;

    private int _timeout = 2000;
    private boolean _decrypt = true;
    private boolean _userAuth = true;
    private boolean _randomNonce = false;
    private boolean _strongBox = true;

    private View _view;
    private Switch _switchAuth;
    private Switch _switchDecrypt;
    private Switch _switchNonce;
    private Switch _switchStrongBox;
    private LinearLayout _layoutTimeout;
    private EditText _textTimeout;

    private TextView _log;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        _log = findViewById(R.id.log);

        showOptionsDialog();
    }

    private void showOptionsDialog() {
        _view = getLayoutInflater().inflate(R.layout.dialog_options, null);
        _switchAuth = _view.findViewById(R.id.switch_auth);
        _switchDecrypt = _view.findViewById(R.id.switch_decrypt);
        _switchNonce = _view.findViewById(R.id.switch_nonce);
        _switchStrongBox = _view.findViewById(R.id.switch_strongBox);
        _layoutTimeout = _view.findViewById(R.id.layout_timeout);
        _textTimeout = _view.findViewById(R.id.text_timeout);
        _switchAuth.setChecked(_userAuth);
        _switchAuth.setOnCheckedChangeListener((buttonView, isChecked) -> {
            _layoutTimeout.setVisibility(isChecked ? View.GONE : View.VISIBLE);
        });
        _switchDecrypt.setChecked(_decrypt);
        _switchNonce.setChecked(_randomNonce);
        _switchStrongBox.setChecked(_strongBox);
        _textTimeout.setText(Integer.toString(_timeout));

        new AlertDialog.Builder(this)
                .setView(_view)
                .setTitle("Options")
                .setPositiveButton(android.R.string.ok, new OptionsListener())
                .show();
    }

    private void startEncrypt() {
        _log.append("\n");

        try {
            Cipher cipher = Cipher.getInstance("AES/GCM/NoPadding");
            if (_randomNonce) {
                cipher.init(Cipher.ENCRYPT_MODE, _key);
            } else {
                byte[] nonce = ByteBuffer.allocate(12).order(ByteOrder.BIG_ENDIAN).putInt(8, i++).array();
                cipher.init(Cipher.ENCRYPT_MODE, _key, new GCMParameterSpec(16 * 8, nonce));
            }

            if (_userAuth) {
                BiometricPrompt.CryptoObject obj = new BiometricPrompt.CryptoObject(cipher);
                BiometricPrompt prompt = new BiometricPrompt.Builder(this)
                        .setTitle("Encrypt")
                        .setNegativeButton("Cancel", getMainExecutor(), (dialog, which) -> {
                        })
                        .build();

                prompt.authenticate(obj, new CancellationSignal(), getMainExecutor(), new BiometricPrompt.AuthenticationCallback() {
                    @Override
                    public void onAuthenticationSucceeded(BiometricPrompt.AuthenticationResult result) {
                        encrypt(result.getCryptoObject().getCipher());
                    }
                });
            } else {
                new Handler().postDelayed(() -> encrypt(cipher), _timeout);
            }
        } catch (Exception e) {
            logError(e);
            showOptionsDialog();
        }
    }

    private void startDecrypt(byte[] encrypted, byte[] nonce) {
        try {
            Cipher cipher = Cipher.getInstance("AES/GCM/NoPadding");
            cipher.init(Cipher.DECRYPT_MODE, _key, new GCMParameterSpec(16 * 8, nonce));

            if (_userAuth) {
                BiometricPrompt.CryptoObject obj = new BiometricPrompt.CryptoObject(cipher);
                BiometricPrompt prompt = new BiometricPrompt.Builder(this)
                        .setTitle("Decrypt")
                        .setNegativeButton("Cancel", getMainExecutor(), (dialog, which) -> {

                        })
                        .build();

                prompt.authenticate(obj, new CancellationSignal(), getMainExecutor(), new BiometricPrompt.AuthenticationCallback() {
                    @Override
                    public void onAuthenticationSucceeded(BiometricPrompt.AuthenticationResult result) {
                        decrypt(result.getCryptoObject().getCipher(), encrypted);
                    }
                });
            } else {
                decrypt(cipher, encrypted);
            }
        } catch (NoSuchAlgorithmException
                | InvalidKeyException
                | InvalidAlgorithmParameterException
                | NoSuchPaddingException e) {
            logError(e);
            showOptionsDialog();
        }
    }

    private void encrypt(Cipher cipher) {
        byte[] plain = "this is a test string".getBytes(StandardCharsets.UTF_8);

        byte[] encrypted;
        try {
            encrypted = cipher.doFinal(plain);
        } catch (BadPaddingException | IllegalBlockSizeException e) {
            logError(e);
            showOptionsDialog();
            return;
        }

        byte[] cipherText = Arrays.copyOfRange(encrypted, 0, encrypted.length - 16);
        byte[] tag = Arrays.copyOfRange(encrypted, encrypted.length - 16, encrypted.length);
        log(String.format("plaintext: %s", encode(plain)));
        log(String.format("ciphertext: %s", encode(cipherText)));
        log(String.format("tag: %s", encode(tag)));
        log(String.format("nonce: %s", encode(cipher.getIV())));

        if (_decrypt) {
            startDecrypt(encrypted, cipher.getIV());
        } else {
            startEncrypt();
        }
    }

    private void decrypt(Cipher cipher, byte[] encrypted) {
        byte[] decrypted;
        try {
            decrypted = cipher.doFinal(encrypted);
        } catch (BadPaddingException | IllegalBlockSizeException e) {
            logError(e);
            showOptionsDialog();
            return;
        }

        log(String.format("decrypted: %s", encode(decrypted)));
        startEncrypt();
    }

    private void log(String msg) {
        Log.println(Log.DEBUG, "Main", msg);
        _log.append(msg + "\n");
    }

    private void logError(Exception e) {
        Toast.makeText(this, e.toString(), Toast.LENGTH_LONG).show();

        StringWriter sw = new StringWriter();
        e.printStackTrace(new PrintWriter(sw));
        e.printStackTrace();
        log("\n" + sw.toString());
    }

    private static String encode(byte[] a) {
        StringBuilder sb = new StringBuilder(a.length * 2);
        for (byte b : a) {
            sb.append(String.format("%02x", b));
        }
        return sb.toString();
    }

    private class OptionsListener implements DialogInterface.OnClickListener {
        @Override
        public void onClick(DialogInterface dialog, int which) {
            _userAuth = _switchAuth.isChecked();
            _decrypt = _switchDecrypt.isChecked();
            _randomNonce = _switchNonce.isChecked();
            _strongBox = _switchStrongBox.isChecked();
            _timeout = Integer.parseInt(_textTimeout.getText().toString());

            try {
                KeyStore store = KeyStore.getInstance("AndroidKeyStore");
                store.load(null);

                // make sure the key store is empty before continuing
                for (String alias : Collections.list(store.aliases())) {
                    store.deleteEntry(alias);
                }

                int purpose = KeyProperties.PURPOSE_ENCRYPT;
                if (_decrypt) {
                    purpose |= KeyProperties.PURPOSE_DECRYPT;
                }

                KeyGenerator generator = KeyGenerator.getInstance(KeyProperties.KEY_ALGORITHM_AES, "AndroidKeyStore");
                generator.init(new KeyGenParameterSpec.Builder("test", purpose)
                        .setBlockModes(KeyProperties.BLOCK_MODE_GCM)
                        .setEncryptionPaddings(KeyProperties.ENCRYPTION_PADDING_NONE)
                        .setRandomizedEncryptionRequired(_randomNonce)
                        .setUserAuthenticationRequired(_userAuth)
                        .setIsStrongBoxBacked(_strongBox)
                        .setKeySize(256)
                        .build());

                _key = generator.generateKey();
            } catch (Exception e) {
                logError(e);
                showOptionsDialog();
                return;
            }

            startEncrypt();
        }
    }
}
