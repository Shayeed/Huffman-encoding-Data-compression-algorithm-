# Huffman-encoding-Data-compression-algorithm-

Overview :- Assume that the data consists of stream of symbols from a finite alphabet. Compression algorithms encode this data such that it can be transmitted/stored with minimum size. Huffman coding is a lossless data compression algorithm developed by David A. Huffman while he was a PhD student at MIT. It forms the basis of the ’zip’ file format. It has several interesting properties:

• It is a variable length code. Frequent symbols are encoded using shorter codes.
• It is aprefix code. No code occurs as a prefix of another code. This property is also called as instant decoding. The code can be decoded using current and past inputs only.
• It is the best possible prefix code(on an average)–it produces the shortest prefix code for a given symbol frequency.

This project implments the huffman coding in C language by reading in data from a text file and generating a frequency file and encoded file. It can also decode the encoded file using the frequency file. There are two seprate folders (1)Encoder and (2)Decoder. The instructions are given in the instructions file.

Things that I learnt from implementing this algorithm are :-
1. Binary heap (to be used as a priority queue)
2. Huffman algorithm (especially Huffman tree creation)
3. File handling and manipulation in C
