package workshop.eleven;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class CsvReaderTest {

    @Test
    public void testValidCsv() throws IOException, CsvException {
        StringReader testReader = new StringReader(
                "student,mark\n"
                        + "Alice,90\n"
                        + "Bob,65\n"
                        + "Carol,72\n"
                        + "Dick,\"12,24,48\""
        );
        String filename = "test.csv";

        List<List<String>> expected = List.of(
                List.of("student", "mark"),
                List.of("Alice", "90"),
                List.of("Bob", "65"),
                List.of("Carol", "72"),
                List.of("Dick", "\"12,24,48\"")
        );

        CsvReader csvReader = new CsvReader();
        assertEquals(expected, csvReader.parseCsv(testReader, filename));
    }

    @Test
    public void testEmptyCsv() {
        StringReader testReader = new StringReader(
                ""
        );
        String filename = "test.csv";

        CsvReader csvReader = new CsvReader();
        var thrown = assertThrows(
                CsvException.class,
                () -> csvReader.parseCsv(testReader, filename)
        );

        assertTrue(thrown.getMessage().contains("empty"));
    }

    @Test
    public void testUnmatchedQuote() {
        StringReader testReader = new StringReader(
                "student,mark\n"
                        + "Alice,90\n"
                        + "\"Bob,yes\",65\n"
                        + "\"Carol,72"
        );
        String filename = "test.csv";

        CsvReader csvReader = new CsvReader();
        var thrown = assertThrows(
                UnmatchedQuoteException.class,
                () -> csvReader.parseCsv(testReader, filename)
        );
        assertTrue(thrown.getMessage().contains("row 3") && thrown.getMessage().contains("column 0"));
    }

    @Test
    public void testTooManyEntries() {
        StringReader testReader = new StringReader(
                "student,mark\n"
                        + "Alice,90\n"
                        + "Bob,65,10\n"
                        + "Carol,72"
        );
        String filename = "test.csv";

        CsvReader csvReader = new CsvReader();
        var thrown = assertThrows(
                TooManyEntriesException.class,
                () -> csvReader.parseCsv(testReader, filename)
        );
        assertTrue(thrown.getMessage().contains("row 2") && thrown.getMessage().contains("2") && thrown.getMessage().contains("3"));
    }

    @Test
    public void testNotEnoughEntries() {
        StringReader testReader = new StringReader(
                "student,mark\n"
                        + "Alice\n"
                        + "Bob,65\n"
                        + "Carol,72"
        );
        String filename = "test.csv";

        CsvReader csvReader = new CsvReader();
        var thrown = assertThrows(
                NotEnoughEntriesException.class,
                () -> csvReader.parseCsv(testReader, filename)
        );
        assertTrue(thrown.getMessage().contains("row 1") && thrown.getMessage().contains("1") && thrown.getMessage().contains("2"));
    }

}
