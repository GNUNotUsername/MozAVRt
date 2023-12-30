# MozAVRt
Compile sheet music PDFs into machine interpretable music instructions, then play it with piezo buzzers.

This project comes in two parts - Musi-CC and MozAVRt.

# Musi-CC

This program uses a slew of image segmentation and pattern recognition techniques to digest sheet music into a structured and machine-interpretable format. The basic process is as follows:

* A sheet music PDF is read in.
* The PDF is converted to greyscale PNG(s) and concatenated together in the event the original PDF had several pages.
* Contrast is heightened and noisy borders are cropped such that details are easier to distinguish.
* A YOLO-network is used to segment the musical components (notes, accidentals, repeats etc) on each staff and record their positions.
* Each identified musical component is fed into a classifier to convert it into an enumerated/tokenised form.
* Tokens are reassembled in order into a pseudo-abstract syntax tree per instrumental part.
* The syntax tree for each part is "compiled" into a set of wave periods, start timestamps and stop timestamps.
* These tone sets are serialised onto disk using protocol buffers.

The format of the tone set protocol buffers are illustrated in the below diagram:

(Diagram coming soon to a README.md near you)

The code for this can be found in the ```compiler``` directory.

## Usage:

```python musi-CC.py <music path> <pbuf path>```

where ```<music path>``` is the path to the sheet music PDF and ```<pbuf path>``` is the path to where the tone set protocol buffers should be written.

## Intentional Omissions:
* Any staves with a percussion clef are ignored as there is no adequate way of mimicking percussion noises with piezo buzzers.
* Any guitar tablature staves are ignored because tablature is a staggeringly bad way of notating music. Guitarists are to blame for this.
* Dynamics are ignored as the specific piezo buzzers used during development and testing do not respond well enough to duty cycling to adequately recreate changes in dynamics.
* Lyrics under vocal parts are ignored for obvious reasons.
* Any non-standard note heads (cross heads, triangle heads etc) are ignored because their meaning is not consistent between scores.
* Breath marks, caesuras and fermatas are ignored because it is annoying to factor them in to the tone set timings.
* Tenutos, accents, marcatos, pizzicatos, arcos and piano pedaling are ignored because they are meaningless to piezo buzzers.

## Limitations:
* This uses neural networks, which are inherently probabilistic and will eventually get something wrong.
* Grainy or low-quality sheet music (e.g. low resolution scans) will produce less accurate tone sets due to added noise.
* Similarly, low-contrast sheet music (e.g. bad photocopies which get a greyish background) will produce less accurate tone sets.
* Anything silly such as white music written on a black background will almost certainly not work.
* If there is no time signature given in the top left corner of the first page of music, a tempo of 100BPM will be assumed. This may or may not make tunes sound terrible.

## Dependencies:
* pdftoppm
* ImageMagick
* python-subprocess


# MozAVRt

This program is far more straightforward. It deserialises the tone data protocol buffers and transmits the data encapsulated within them to an ATMega324A board over USART. This ATMega324A board then plays the music it receives using eight piezo buzzers. It is assumed the ATMega324A board is wired as per the below schematic:

(Schematic also coming to the same README.md near you)

The code running on the ATMega324A board can be found in the ```embed``` directory and the code to deserialise and transmit the tone set protocol buffers is found in the ```transmission``` directory.

## Transmission Protocol:
The protocol used to transmit the tone set to the AVR board over USART is yet again coming soon to the same README.md near you.

## Usage:

TBD

## Notes:

None yet

## Dependencies
* AVR-GCC
* AVR-LibC
* AVRDude

# Conjunction

Both Musi-CC and MozAVRt can be run at once using a script which is coming later

# Samples

Sample music PDFs can be found in the ```samples``` directory.
