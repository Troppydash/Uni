package workshop.eleven;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class CsvReader {
    public List<List<String>> readCsv(String filename)
            throws IOException, CsvException {
        try (var reader = new FileReader(filename)) {
            return this.parseCsv(reader, filename);
        }
    }

    private List<String> parseLine(BufferedReader reader, int row, String filename) throws IOException, CsvException {
        String line = reader.readLine();
        if (line == null) {
            return null;
        }

        List<String> entries = new ArrayList<>();

        int ptr = 0;
        while (ptr < line.length()) {
            int start = ptr;
            boolean quotes = false;
            while (ptr < line.length()) {
                if (!quotes && line.charAt(ptr) == ',') {
                    entries.add(line.substring(start, ptr));
                    ptr += 1;
                    start = ptr;
                    break;
                }

                if (line.charAt(ptr) == '\"') {
                    quotes = !quotes;
                }

                ptr += 1;
            }

            if (quotes) {
                throw new UnmatchedQuoteException(row, entries.size(), filename);
            }

            // add last
            if (start < ptr) {
                entries.add(line.substring(start, ptr));
            }
        }


        return entries;
    }

    public List<List<String>> parseCsv(Reader reader, String filename)
            throws IOException, CsvException {

        var bufferedReader = new BufferedReader(reader);
        var content = new ArrayList<List<String>>();

        // parse header
        List<String> header = this.parseLine(bufferedReader, 0, filename);
        if (header == null) {
            throw new CsvException("empty header line", filename);
        }
        content.add(header);

        // parse remaining rows
        for (int row = 1; ; ++row) {
            List<String> entries = this.parseLine(bufferedReader, row, filename);
            if (entries == null) {
                break;
            }

            // check for size
            if (entries.size() > header.size()) {
                throw new TooManyEntriesException(row, header.size(), entries.size(), filename);
            }
            if (entries.size() < header.size()) {
                throw new NotEnoughEntriesException(row, header.size(), entries.size(), filename);
            }

            content.add(entries);
        }

        return content;
    }
}
