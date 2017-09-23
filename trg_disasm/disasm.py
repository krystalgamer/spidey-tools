import sys
from decoder import Decoder

class Disassembler():

    def __init__(self, fileName):
        self.curPos = 0 #Cur pos on disasm
        self.numInsn = 0 #Total number of instructions
        self.fileBuffer = self.ReadWholeFile(fileName) #Contains file buffer
        self.decoder = Decoder(self.fileBuffer)
        print(fileName + ' was successfully read!')

    def ReadWholeFile(self, fileName):
        with open(fileName, 'rb') as fp:
            fp.seek(0, 2)
            fileSize = fp.tell()
            fp.seek(0, 0)
            return fp.read(fileSize)

    def ValidateTrgHeaders(self):
        #Checks for TRG header
        if not self.fileBuffer[0:4] == b'_TRG':
            print('Not a TRG file')
            return False
        version = int.from_bytes(self.fileBuffer[4:8], byteorder='little')
        if not ((version & 0xFFFF) == 2) and ((version & 0xFFFF0000) == 0x10000):
            print('Not a spidey trg file')
            return False
        return True

    def PrepareDisasm(self):
        if not self.ValidateTrgHeaders():
            return False

        self.numInsn = int.from_bytes(self.fileBuffer[8:10], byteorder='little')
        print('Number of instructions: ' + str(self.numInsn))

        #Verify the existence of the terminator node
        termPos = 0xC + (self.numInsn - 1) * 4
        termPos = int.from_bytes(self.fileBuffer[termPos: termPos + 2], byteorder='little')
        if not self.fileBuffer[termPos : termPos + 2] == b'\xFF\x00':
            print('Missing terminator node')
            return False

        print('Ready to disasm!')
        return True

    def StartDisasm(self):

        #First part
        for counter in range(self.numInsn):
            tmpPos = 0xC + (counter * 4) 
            tmpPos = int.from_bytes(self.fileBuffer[tmpPos : tmpPos + 4], byteorder='little') 
            if not self.fileBuffer[tmpPos : tmpPos + 2] == b'\x04\x00':
                continue
            
            self.decoder.Decode(tmpPos + 2) 
        #second related to restart node
        #TODO find the restart node position
        #And start of instruction part 
        print('Nova parte')
        self.decoder.Decode(0x1C2)#l4a1 specific
        
        return
        

def disasm(trgFile):

    decoder = Decoder(None)

    disasm = Disassembler(trgFile)
    if not disasm.PrepareDisasm():
        print('Quitting..')
        return
    disasm.StartDisasm()
    
    return


def main():
    if len(sys.argv) < 2:
        print('Please specify a file name')
        return
    disasm(sys.argv[1])

if __name__ == "__main__":
    main()
