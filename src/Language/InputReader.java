import java.util.StringTokenizer;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.InputStream;

static class InputReader {
    private InputStreamReader reader;
    private char[] buf;
    private int len, now;
    
    public InputReader(InputStream stream) {
        reader = new InputStreamReader(stream);
        buf = new char[1024];
        len = 0;
        now = 0;
    }
    
    public String next() throws IOException {
        if (!hasNext()) throw new NullPointerException();
        StringBuilder sb = new StringBuilder();
        while (!isSpaceChar(buf[now])) {
            sb.append(buf[now]);
            if (!move()) break;
        }
        return sb.toString();
    }
    
    public int nextInt() throws IOException {
        if (!hasNext()) throw new NullPointerException();
        boolean x = false;
        if (buf[now] == '-') {
            x = true;
            if (!move()) throw new NumberFormatException();
        }
        int ans = 0;
        while (!isSpaceChar(buf[now])) {
            if (isNum(buf[now])) ans = ans * 10 + buf[now] - '0';
            else throw new NumberFormatException();
            if (!move()) break;
        }
        return (x ? -1 : 1) * ans;
    }
    
    public String nextLine() throws IOException {
        if (!hasNextLine()) throw new NullPointerException();
        StringBuilder sb = new StringBuilder();
        while (buf[now] != '\n') {
            sb.append(buf[now]);
            if (!move()) return sb.toString();
        }
        now++;
        return sb.toString();
    }
    
    public long nextLong() throws IOException {
        if (!hasNext()) throw new NullPointerException();
        boolean x = false;
        if (buf[now] == '-') {
            x = true;
            if (!move()) throw new NumberFormatException();
        }
        long ans = 0;
        while (!isSpaceChar(buf[now])) {
            if (isNum(buf[now])) ans = ans * 10 + buf[now] - '0';
            else throw new NumberFormatException();
            if (!move()) break;
        }
        return (x ? -1 : 1) * ans;
    }
    
    public double nextDouble() throws IOException {
        return Double.parseDouble(next());
    }
    
    public int nextHexInt() throws IOException {
        if (!hasNext()) throw new NullPointerException();
        boolean x = false;
        if (buf[now] == '-') {
            x = true;
            if (!move()) throw new NumberFormatException();
        }
        int ans = 0;
        while (!isSpaceChar(buf[now])) {
            if (isHex(buf[now])) ans = ans * 16 + toHex(buf[now]);
            else throw new NumberFormatException();
            if (!move()) break;
        }
        return (x ? -1 : 1) * ans;
    }
    
    public char nextChar() throws IOException {
        if (!hasNext()) throw new NullPointerException();
        char tmp = buf[now];
        move();
        return tmp;
    }

    public int next(char[] s) throws IOException {
        if (!hasNext()) throw new NullPointerException();
        int len=0;
        while (!isSpaceChar(buf[now])&&len<s.length) {
            s[len++]=buf[now];
            if (!move()) break;
        }
        return len;
    }

    public boolean hasNext() throws IOException {
        return skip();
    }
    
    public boolean hasNextLine() throws IOException {
        return now < len || refill();
    }
    
    private boolean move() throws IOException {
        now++;
        return hasNextLine();
    }
    
    private boolean skip() throws IOException {
        if (!hasNextLine()) return false;
        while (isSpaceChar(buf[now])) {
            if (!move()) return false;
        }
        return true;
    }
    
    private boolean isSpaceChar(char c) {
        return !(c >= 33 && c <= 126);
    }
    
    private boolean isNum(char c) {
        return c >= '0' && c <= '9';
    }
    
    private boolean isHex(char c) {
        return c >= '0' && c <= '9' || c >= 'A' && c <= 'F';
    }
    
    private int toHex(char c) {
        if (c >= '0' && c <= '9') return c - '0';
        else return c - 'A' + 10;
    }
    
    private boolean refill() throws IOException {
        len = reader.read(buf);
        now = 0;
        return len > 0;
    }
}