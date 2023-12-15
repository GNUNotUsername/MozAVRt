# MozAVRt
Compile sheet music PDFs into machine interpretable music instructions, then play it with piezo buzzers.

This project comes in two parts - Musi-CC and MozAVRt.

# Musi-CC

This program uses a slew of image segmentation and pattern recognition techniques to digest sheet music into a structured and machine-interpretable format. The basic process is as follows:

* A sheet music PDF is read in.
* The PDF is converted to greyscale PNG(s) and concatenated together in the event the original PDF had several pages.
* Contrast is heightened such that details are easier to distinguish.
* A vertical column on each page is scanned to create rough boundaries around the musical staves.
* A YOLO-network is used to segment the musical components (notes, accidentals, repeats etc) on each staff and record their positions.
* Each identified musical component is fed into a classifier CNN to convert it into an enumerated/tokenised form.
* Tokens are reassembled in order, into a pseudo-abstract syntax tree per instrumental part.
* The syntax tree for each part is "compiled" into a set of frequencies, start timestamps and stop timestamps.
* These tone sets are serialised onto disk using protocol buffers.

The format of the tone set protocol buffers are illustrated in the below diagram:

(Diagram coming soon to a README.md near you)

The code for this can be found in the ```compiler``` directory.

## Usage:

```python musi-CC.py <music path> <pbuf path>```

where ```<music path>``` is the path to the sheet music PDF and ```<pbuf path>``` is the path to where the tone set protocol buffers should be written.

## Notes:
* Some details are intentionally discarded by the compiler.
* Any staves with a percussion clef are ignored as there is no adequate way of mimicking percussion noises with piezo buzzers.
* Any guitar tablature staves are ignored because tablature is a staggeringly crap way of notating music. Guitarists are to blame for this.
* Dynamics are ignored as the specific piezo buzzers used during development and testing do not respond well enough to duty cycling to adequately recreate changes in dynamics.
* Lyrics under vocal parts are ignored obvious reasons.
* Any non-standard note heads (cross heads, triangle heads etc) are ignored because their meaning is not consistent between scores.


# MozAVRt

This program is far more straightforward. It deserialises the tone data protocol buffers and transmits the data encapsulated within them to an ATMega324A board over USART. This ATMega324A board then plays the music it receives using eight piezo buzzers. It is assumed the ATMega324A board is wired as per the below schematic:

(Schematic also coming to the same README.md near you)

The code running on the ATMega324A board can be found in the ```embed``` directory and the code to deserialise and transmit the tone set protocol buffers is found in the ```transmission``` directory.

## Usage:

TBD

## Notes:

None yet

# Conjunction

Both Musi-CC and MozAVRt can be run at once using a script which is coming later
