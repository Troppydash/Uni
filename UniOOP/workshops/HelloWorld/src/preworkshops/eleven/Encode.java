package preworkshops.eleven;

import java.util.HashMap;

public class Encode {
    public static String shift(String text, int shift) {
        String out = "";
        for (var c : text.toCharArray()) {
            if (Character.isAlphabetic(c)) {
                out += Character.toString(Math.floorMod(Character.toLowerCase(c)-'a'+shift, 26)+'a');
            } else if (c == ' ') {
                out += ' ';
            }
        }
        return out;
    }

    public static void main(String[] args) {
        String original = "I've come to make an announcement: Shadow the Hedgehog's a bitch ass mother fucker. He pissed on my fucking wife. That's right, he took his hedgehog fuckin' quilly dick out and he pissed on my fucking wife, and he said his dick was THIS BIG. And I said “that's disgusting!” So I'm making a callout post on my twitter dot com: \\\"Shadow the Hedgehog, you got a small dick, it's the size of this walnut except WAY smaller.\\\" And guess what, here's what my dong looks like: PFFFFFFFFGJT. That's right baby. All point, no quills, no pillows, look at that it looks like two balls and a bong. He fucked my wife so guess what, I'm gonna FUCK THE EARTH. THATS RIGHT THIS IS WHAT YOU GET, MY SUPER LAZER PISS. Except I'm not gonna piss on the earth, I'm gonna go higher. I'm pissing on the MOOOOOON! How do you like that, OBAMA? I PISSED ON THE MOON, YOU IDIOT! You have twenty-three hours before the piss d r o p l e t s hit the fucking earth, now get out of my fucking sight before I piss on you too!";
        var out = shift(original, 69);
        System.out.println(out);
        System.out.println(shift(out,-69));
    }
}
