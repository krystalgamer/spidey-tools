import sys
from decoder import Decoder

class Disassembler():

    def __init__(self, fileName):
        self.curPos = 0 #Cur pos on disasm
        self.numInsn = 0 #Total number of instructions
        self.restartNode = 0
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

        #Second part(Exec autoexec)
        for counter in range(self.numInsn):
            tmpPos = 0xC + (counter * 4) 
            tmpPos = int.from_bytes(self.fileBuffer[tmpPos : tmpPos + 4], byteorder='little') 
            if not self.fileBuffer[tmpPos : tmpPos + 2] == b'\x04\x00':
                continue
            
            print('Executing AUTOEXEC')
            self.decoder.Decode(tmpPos + 2) 

        #Third part parse TRG
        for counter in range(self.numInsn):
            tmpPos = 0xC + (counter * 4) 
            tmpPos = int.from_bytes(self.fileBuffer[tmpPos : tmpPos + 4], byteorder='little') 
            command = int.from_bytes(self.fileBuffer[tmpPos : tmpPos + 2], byteorder='little')

            #command>0x14 doesnt do shit so ignore it(calls DoAssert)
            #command==0x14 same shit(Trig_GetPosition but never uses the result) 
            if command < 0x14:
                if command == 1:
                    #Makes sure there's a terminator node
                    v4 = tmpPos + 2 * int.from_bytes(self.fileBuffer[tmpPos + 6: tmpPos + 8], byteorder='little') + 8
                    while self.fileBuffer[v4] != b'\xFF':
                        #Crashes if there's no terminator part
                        v4+=1
                if command command:w


            
            print('Executing AUTOEXEC')
            self.decoder.Decode(tmpPos + 2) 


        return

        #TODO find the restart node position
        if self.restartNode == 0xFFFF:
            print('No restart node set... Quitting')
            return
        nodePos = 0xC + 4 * self.restartNode 
        if self.fileBuffer[nodePos : nodePos + 2] != b'\x08\x00':
            print('Not a restart node? Not running')
            return

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
