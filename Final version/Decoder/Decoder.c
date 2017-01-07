// C program for decoding the file
#include <stdio.h>
#include <stdlib.h>

//------------------------------------Structures and Global variable--------------------------------------------- 
// A Huffman tree node
struct MinHeapNode
{
    char data;  // One of the input characters
    unsigned freq;  // Frequency of the character
    struct MinHeapNode *left, *right; // Left and right child of this node
};
 
// A Min Heap:  Collection of min heap (or Hufmman tree) nodes
struct MinHeap
{
    unsigned size;    // Current size of min heap
    unsigned capacity;   // capacity of min heap
    struct MinHeapNode **array;  // Array of minheap node pointers (Double pointer is for creating array)
};

//Structure to hold frequency of characters read from file
struct frequency
{
   char character;  //Character holder
   int frequency;  //Frequency holder
};

//Structure to hold character codes
struct Holder
{
   char character;  //Character holder
   char code[255];  //Code holder
};

struct frequency CharFrequency[255]; //Global variable to hold chracter and frequency from file
int FrequencyCounter=-1;

struct Holder TotalChars[255]; //Global variable to hold chracter and codes of huffman tree
int CharacterCounter=-1;

//-----------------------------------------------------------------------------------------------------------

//--------------------------------Functions for Min Heap-----------------------------------------------------
 
// A utility function allocate a new min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(char data, unsigned freq)
{
    struct MinHeapNode* temp =
          (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}
 
// A utility function to create a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->size = 0;  // current size is 0
    minHeap->capacity = capacity;
    minHeap->array =
     (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
 
// A utility function to swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;   //Gives array index of left child
    int right = 2 * idx + 2;  //Gives array index of right child
 
    if (left < minHeap->size &&
        minHeap->array[left]->freq < minHeap->array[smallest]->freq)
      smallest = left;
 
    if (right < minHeap->size &&
        minHeap->array[right]->freq < minHeap->array[smallest]->freq)
      smallest = right;
 
    if (smallest != idx)
    {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}
 
// A utility function to check if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}
 
// A standard function to extract minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}
 
// A utility function to insert a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode)
{
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1)/2]->freq)   // It means while is is not zero
    {
        minHeap->array[i] = minHeap->array[(i - 1)/2];
        i = (i - 1)/2;
    }
    minHeap->array[i] = minHeapNode;
}
 
// A standard funvtion to build min heap
void buildMinHeap(struct MinHeap* minHeap)
{
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}
 
// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)
{
    return !(root->left) && !(root->right) ;
}
 
// Creates a min heap of capacity equal to size and inserts all character of 
// data[] in min heap. Initially size of min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap()
{
    int i;
    struct MinHeap* minHeap = createMinHeap(FrequencyCounter+1);
    for (i = 0; i <= FrequencyCounter; ++i)
        minHeap->array[i] = newNode(CharFrequency[i].character, CharFrequency[i].frequency);
    minHeap->size = FrequencyCounter+1;
    buildMinHeap(minHeap);
    return minHeap;
}

//-----------------------------------------------------------------------------------------------------------

//--------------------------------Functions for Huffman Tree-------------------------------------------------
 
// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree()
{
    struct MinHeapNode *left, *right, *top;
 
    // Step 1: Create a min heap of capacity equal to size.  Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap();
 
    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap))
    {
        // Step 2: Extract the two minimum freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);
 
        // Step 3:  Create a new internal node with frequency equal to the
        // sum of the two nodes frequencies. Make the two extracted node as
        // left and right children of this new node. Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
 
    // Step 4: The remaining node is the root node and the tree is complete.
    return extractMin(minHeap);
}
  
// The main function that builds a Huffman Tree and print codes by traversing
// the built Huffman Tree
struct MinHeapNode* HuffmanCodes()
{
   //  Construct Huffman Tree
   struct MinHeapNode* root = buildHuffmanTree();
   printf("\nHuffman tree build successfull. Press enter to continue.....");
   getch();
   return root;
}

//-----------------------------------------------------------------------------------------------------------

//-------------------------------------Functions to decode data----------------------------------------------
//Function to decode the data
void decode(struct MinHeapNode* root)
{
   struct MinHeapNode* TempRoot;
   TempRoot=root;    //Create a temporary root node to traverse the tree 
   char c;
   fflush(stdin);  //Used to clear the input buffer beacuse the gets code is taking it and not taking any value
   
   FILE *pFile1;  //Pointer to file
   pFile1 = fopen("encode_out.txt","r");  //Open file to read data
    
   FILE *pFile2;  //Pointer to file
   pFile2 = fopen("decode_out.txt","w");  //Open file to read data
   
   if((pFile1!=NULL)&&(pFile2!=NULL))
   {                            
       while(1)
       {
          if( feof(pFile1) )  //tests the end-of-file indicator for the given stream
          {
             break;
          }
          else
          {
              c = fgetc(pFile1);  //Gets charcater from file
              
              if(c==EOF)
                 break;
                 
              if(!isLeaf(TempRoot))
              {
                  if(c=='0')  //If character is 0 move root to left
                  {
                     TempRoot=TempRoot->left;
                  }
                  else if(c=='1')   //If character is 1 move root to right
                  {
                     TempRoot=TempRoot->right;
                  }
                  else
                  {
                     printf(".....Decoding unsuccessful. Data incorrect\n");  //If any other data encountered print corrupt data
                     break;
                  }
              }      
              
              if(isLeaf(TempRoot))  //If leaf encountered charcter is reached print it.
              {
                  fputc(TempRoot->data,pFile2);   //Put chracter in file
                  TempRoot = root;   //Temp root is again given value of root i.e starting point
              }
          }
       }
       if(TempRoot != root)  //If after all traversals some code is left it means error has occured
           printf("\nData decode unsuccessfull. Incorrect input");
           
       fclose(pFile1); //Close file
       fclose(pFile2);
       printf("\nDecoding successfull. Press enter to exit");
       getch();
   }
   else
   {
      printf("\nFile not found. Press enter to exit...");
      getch();
      exit(0);
   }
}


//Funtion to read frequency file and write data to frequency structure
void FrequencyBuilder()
{
     FILE *FreqyencyFile;
     FreqyencyFile = fopen("Frequency.bin","rb");
     if(FreqyencyFile!=NULL)
     {	
    	fseek(FreqyencyFile, 0, SEEK_END);  //Move pointer to end of file
    	long fileSize = ftell(FreqyencyFile);   //Finds size of file
    	rewind(FreqyencyFile);    //Move pointer to startin of file
    	
    	int numStructures = (int)(fileSize / (sizeof(struct frequency)));   //Find the number of structures in file
    	
    	size_t returnValue;  //Number of structures read
    	returnValue = fread(CharFrequency, sizeof(struct frequency), numStructures, FreqyencyFile );  //Read the file in structures
    	FrequencyCounter=returnValue-1;  //Allot counter for frequency structure
    	fclose(FreqyencyFile);
    	printf("Frequency file read successfull. Print enter to continue...");
        getch();
     }
     else
     {
     printf("File not found. Press enter to exit...");
     getch();
     exit(0);
     }
}   

 
// Driver program to test above functions
int main()
{    
   FrequencyBuilder();  //Reads file and builds structure for frequency
   
   struct MinHeapNode* root;
   root = HuffmanCodes(); //Create huffman tree
       
   decode(root);  //Funtion to decode file
   
   getch();   
   return 0;
}
