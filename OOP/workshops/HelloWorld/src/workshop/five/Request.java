package workshop.five;

public abstract class Request {
    protected final short port;
    public final String address;
    public final String file;

    protected Request(short port, String address, String file) {
        this.port = port;
        this.address = address;
        this.file = file;
    }

    public short getPort() {
        return this.port;
    }

    public abstract String getFullRequest();

}

class HttpRequest extends Request {
    private static final short PORT = 80;
    public final String method;

    public HttpRequest(String address, String file, String method) {
        super(PORT, address, file);

        this.method = method;
    }

    @Override
    public String getFullRequest() {
        return String.format("%s %s HTTP/1.1\r\n\r\n", method, file);
    }
}

class FtpRequest extends Request {
    private static final short PORT = 21;

    public FtpRequest(String address, String file) {
        super(PORT, address, file);
    }

    @Override
    public String getFullRequest() {
        return String.format("RETR %s\u0015\u0012", file);
    }
}

class Test {
    public static void main(String[] args) {
        String type = args[0];

        Request request;
        switch (type) {
            case "http": {
                request = new HttpRequest(args[1], args[3], args[2]);
                break;
            }
            case "ftp": {
                request = new FtpRequest(args[1], args[2]);
                break;
            }
            default: {
                return;
            }
        }

        System.out.println(request.getFullRequest());
    }
}