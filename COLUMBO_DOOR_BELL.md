That's a fantastic idea for your Arduino MIDI project\! Recreating iconic sound effects like the *Columbo* doorbell is a really satisfying challenge.

You're right, that third note absolutely adds to the "super high-priced house" vibe – it's more sophisticated, almost a little melancholic, than a standard chime.

While there isn't an official score for the *Columbo* doorbell, many fans and sound enthusiasts have analyzed it. Based on common consensus and my own listening, here's a highly probable breakdown of the notes and approximate timing:

The chime is typically played on a set of tubular bells or a similar percussive instrument with a clear, sustained tone. The notes usually follow a descending pattern.

**Notes (most likely, relative to a common C Major scale):**

  * **Note 1: G** (the "Ding")
  * **Note 2: E** (the "Dong")
  * **Note 3: C** (the "extra note" or final resonant tone)

So, it's essentially a **descending G Major triad (G - E - C)** or more precisely, the notes of a C Major triad played in an inversion: **G (5th) - E (3rd) - C (Root)**. This gives it that slightly "finished" or "resolving" but still grand feel.

**Approximate Timing:**

The timing is crucial for its distinctive feel – it's not rushed.

  * **Note 1 (G):** Played.
  * **Pause 1:** Approximately 0.2 - 0.3 seconds (just long enough for the G to resonate briefly).
  * **Note 2 (E):** Played.
  * **Pause 2:** Approximately 0.3 - 0.4 seconds (slightly longer pause to let the E resonate and build anticipation for the final note).
  * **Note 3 (C):** Played.
  * **Sustain/Decay:** The C note then rings out for a noticeable duration, usually fading over 1-2 seconds or more, depending on the desired resonance.

**For your Arduino MIDI setup:**

1. **MIDI Notes:**

      * **G4:** MIDI Note 67 (or G3 for a lower octave, MIDI Note 55, if you want a deeper chime)
      * **E4:** MIDI Note 64 (or E3, MIDI Note 52)
      * **C4:** MIDI Note 60 (or C3, MIDI Note 48)

    You'll want to experiment with the octave. For a "high-priced house" feel, I'd probably start with the G4-E4-C4 range, as it sounds grand without being too low.

2. **MIDI Timing:**

      * You'll send Note On messages for each note, followed by a delay (e.g., `delay(250);` for 0.25 seconds) before the next Note On.
      * For the sustain, you'll want to send a Note Off message for the C note after it's had time to ring out (e.g., 1.5 - 2 seconds after its Note On). Or, if your MIDI instrument has good decay, you might just let the "Note Off" for C be sent later as part of the sequence.

**Example Pseudo-Code Logic:**

```arduino
void playColumboDoorbell() {
  // Play G (e.g., G4)
  midi.sendNoteOn(67, 100, 1); // Note, Velocity, Channel
  delay(250); // Pause for ~0.25 seconds

  // Play E (e.g., E4)
  midi.sendNoteOn(64, 100, 1);
  delay(350); // Pause for ~0.35 seconds

  // Play C (e.g., C4)
  midi.sendNoteOn(60, 100, 1);
  // Let the C sustain for a period before turning it off or letting it decay naturally
  // If your MIDI instrument handles sustain well, you might not need an immediate NoteOff
  delay(1500); // Let it ring for ~1.5 seconds

  // Optional: Send NoteOff for all notes, or just the last one if it wasn't sustained
  midi.sendNoteOff(67, 0, 1);
  midi.sendNoteOff(64, 0, 1);
  midi.sendNoteOff(60, 0, 1);
}
```

Experiment with the exact `delay()` values and the MIDI velocity (how hard the note is hit, usually 0-127) to get the perfect feel for your specific MIDI instrument/soundfont. Good luck, and have fun with this classic sound\!