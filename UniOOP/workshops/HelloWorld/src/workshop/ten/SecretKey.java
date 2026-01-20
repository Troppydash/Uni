package workshop.ten;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;

public abstract class SecretKey {
    private byte[] key;

    public SecretKey(int n) {
        this.key = new byte[n];
        ThreadLocalRandom.current().nextBytes(this.key);
    }

    public byte[] encrypt(byte[] message) {
        int m = message.length;
        int k = this.key.length;

        byte[] result = new byte[m];
        for (int i = 0; i < m; ++i) {
            result[i] = (byte) (message[i] ^ this.key[i % k]);
        }

        return result;
    }

    public String decrypt(byte[] message) {
        byte[] text = this.encrypt(message);
        return new String(text, StandardCharsets.UTF_8);
    }
}


class ShortSecretKey extends SecretKey {
    public ShortSecretKey() {
        super(16);
    }
}

class LongSecretKey extends SecretKey {
    public LongSecretKey() {
        super(32);
    }
}

abstract class Encryptor {
    private final SecretKey key;

    public Encryptor() {
        this.key = this.createKey();
    }

    protected abstract SecretKey createKey();

    public byte[] encrypt(List<String> lines) {
        ByteArrayOutputStream output = new ByteArrayOutputStream();

        for (var line : lines) {
            byte[] encrypted = this.key.encrypt(line.getBytes(StandardCharsets.UTF_8));
            try {
                output.write(encrypted);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        return output.toByteArray();
    }
}

class InsecureEncryptor extends Encryptor {
    @Override
    protected SecretKey createKey() {
        return new ShortSecretKey();
    }
}

class SecureEncryptor extends Encryptor {
    @Override
    protected SecretKey createKey() {
        return new LongSecretKey();
    }
}


