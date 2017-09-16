
class Decoder():

    def __init__(self, fileBuffer):
        self.opList = None
        self.fileBuffer = fileBuffer
        self.curInsn = 0

    def SetRestart(self):
       #TODO implement thing with 8 insn 
       #find end of string
       endString = 0
       for b in self.fileBuffer[self.curInsn:]:
           endString+=1
           if b == 0:
               break

       print('SetRestart %s' % self.fileBuffer[self.curInsn : self.curInsn + endString].decode('utf-8'))

       self.curInsn += endString 
       self.curInsn += (self.curInsn & 1)

       return

    def EndLevelNode(self):
       print('EndLevelNode %d' % int.from_bytes(self.fileBuffer[self.curInsn : self.curInsn + 2], byteorder='little'))
       self.curInsn+=2
       return
   
    def SetGameLevel(self):
       print('SetGameLevel %d' % int.from_bytes(self.fileBuffer[self.curInsn : self.curInsn + 2], byteorder='little'))
       self.curInsn+=2
       return

    def RunCinema(self):
       #TODO looks like the interpt script function is not only called in 4th cases
       print('RunCinema %d' % int.from_bytes(self.fileBuffer[self.curInsn : self.curInsn + 2], byteorder='little'))
       self.curInsn+=2
       return
   
    def SpoolIn(self):
       #TODO requires deeper look
       endString = 0
       for b in self.fileBuffer[self.curInsn:]:
           endString+=1
           if b == 0:
               break

       print('SpoolIn %s' % self.fileBuffer[self.curInsn : self.curInsn + endString].decode('utf-8'))

       self.curInsn += endString 
       self.curInsn += (self.curInsn & 1)
       return

    def SpoolEnv(self):
       #TODO just as SpoolIn involves PSXLoad so might be worth looking at it
       endString = 0
       for b in self.fileBuffer[self.curInsn:]:
           endString+=1
           if b == 0:
               break

       print('SpoolEnv %s' % self.fileBuffer[self.curInsn : self.curInsn + endString].decode('utf-8'))

       self.curInsn += endString 
       self.curInsn += (self.curInsn & 1)
       return

    def SetObjFile(self):
       #TODO more file loading... 
       endString = 0
       for b in self.fileBuffer[self.curInsn:]:
           endString+=1
           if b == 0:
               break

       print('SetObjFile %s' % self.fileBuffer[self.curInsn : self.curInsn + endString].decode('utf-8'))

       self.curInsn += endString 
       self.curInsn += (self.curInsn & 1)
       return

    def SpoolCodeModule(self):
       #TODO ... 
       endString = 0
       for b in self.fileBuffer[self.curInsn:]:
           endString+=1
           if b == 0:
               break

       print('SpoolCodeModule %s' % self.fileBuffer[self.curInsn : self.curInsn + endString].decode('utf-8'))

       self.curInsn += endString 
       self.curInsn += (self.curInsn & 1)
       return

    def BackgroundOff(self):
       print('BackgroundOff %d' % int.from_bytes(self.fileBuffer[self.curInsn : self.curInsn + 2], byteorder='little'))
       self.curInsn+=2
       return

    def Terminate(self):
       return True

    opFuncList = { b'\x8C\x00' : SetRestart,
            b'\x2E\x01' : EndLevelNode, b'\x93\x00' : SetGameLevel,
            b'\xBE\x00' : RunCinema, b'\x7E\x00' : SpoolIn,
            b'\x80\x00' : SpoolEnv, b'\x8E\x00' : SetObjFile,
            b'\xBD\x00' : SpoolCodeModule, b'\x84\x00' : BackgroundOff,
            b'\xFF\xFF' : Terminate}

    def Decode(self, index):
       #index of the op code to decode

        self.curInsn = index
        
        decoding = None
        while decoding == None:
            opCode = self.fileBuffer[self.curInsn : self.curInsn + 2]
            self.curInsn += 2
            decoding = self.opFuncList[opCode](self) 

        return
