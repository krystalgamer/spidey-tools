
class Decoder():

    def __init__(self, fileBuffer):
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
    
    def SetFoggingParam(self):
       #TODO parameters are ignored by now
       print('SetFoggingParam (needs to be worked)')
       self.curInsn+=6
       return

    def SetOTPushback2(self):
       print('SetOTPushback2 %d' % int.from_bytes(self.fileBuffer[self.curInsn : self.curInsn + 2], byteorder = 'little') )
       self.curInsn+=2
       return
    def SetOTPushback(self):
       print('SetOTPushback %d' % int.from_bytes(self.fileBuffer[self.curInsn : self.curInsn + 2], byteorder = 'little') )
       self.curInsn+=2
       return

    def SetSpideyCamValue(self):
       #TODO figure the meaning of the values
       print('SetSpideyCamValue')
       self.curInsn += 10
       return

    def AllowCamLOSCheck(self):
       #If different than 0 sets a variable to 1, if not 0
       print('AllowCamLOSCheck %d' % int.from_bytes(self.fileBuffer[self.curInsn : self.curInsn + 2], byteorder = 'little'))
       self.curInsn+=2
       return

    def SetSkyColor(self):
       print('SetSkyColor %x' % int.from_bytes(self.fileBuffer[self.curInsn : self.curInsn + 4], byteorder = 'big'))
       self.curInsn+=4
       return

    def BackgroundCreate(self):
       #TODO how the following bytes are interpreted
       print('BackgroundCreate TBI')
       self.curInsn = ((self.curInsn + 3) & 0xFFFFFFFC) + 10
       return

    def SetBaddyVisibilityInBox(self):
        #TODO implment this
        v60 = self.curInsn + 4
        if not self.fileBuffer[v60 : v60+2] == b'\xFF\x00':
            print('SetBaddyVisibilityInBox not implemented')
            return
        else:
            print('SetBaddyVisibilityInBox implemented')
            self.curInsn = v60 + 2

        return
    def SendPulse(self):
        print('SendPulse partially implemented')

        #v268 needs to be implemented 
        #a2 too
        v268 = 0
        a2 = 0
        if not v268:
            comPos = 0xC + 4 * a2
            comPos = int.from_bytes(self.fileBuffer[comPos : comPos + 2], byteorder = 'little')
            command = int.from_bytes(self.fileBuffer[comPos : comPos + 2], byteorder = 'little')
            if command <= 13:
                if command < 12:
                    #improve this
                    if command == 2 or command == 3 or command == 6 or command == 8 or command == 9 or command == 10:
                        return self.ProcessCommandsF(comPos + 2)
                    elif command == 1:
                        print('That path')
                    elif command == 5:
                        print('Another path')
                    else:
                        print("No path")


            if command > 0x3E9:
                if command != 0x3EA:
                    print('I FUCKED')
            else:
                if command >= 0x3E8:
                    print('FIne?')
                if command != 0x14:
                    print('Unrecognized node type')
        return

    def ProcessCommandsF(self, comPos):
        numCom = int.from_bytes(self.fileBuffer[comPos : comPos + 2], byteorder = 'little')
        if numCom <= 0:
            print('I\'ve not implemented it')
            return
        print('There are %d commands' % numCom)

        v249 = comPos + 2
        while numCom:
            v152 = int.from_bytes(self.fileBuffer[v249 : v249 + 2], byteorder = 'little')
            print('Sending pulse to node: %d' % v152)
            v153 = 0xC + 4 * v152
            v153 = int.from_bytes(self.fileBuffer[v153 : v153 + 2], byteorder = 'little')
            v153 = int.from_bytes(self.fileBuffer[v153 : v153 + 2], byteorder = 'little')

            if v153 == 1 or v153 == 5 or v153 == 7 or v153 == 20:
                print('Calls create baddy (implement it)')
                #TODO CreayeBaddy
            elif v153 == 6:
                if v153 != 6:
                    print('God have mercy on me')

                print('Related to pending commands')
            else:
                print('Default case')
                return
            v249+=2
            numCom-=1

        #TODO add missing parts
        print('Command list ended')
        if self.fileBuffer[self.curInsn : self.curInsn + 2] != b'\xFF\xFF':
            print('More instructions to come')
            return#More to come
        #TODO something related to missing if statements
        return True



    opFuncList = { b'\x8C\x00' : SetRestart,
            b'\x2E\x01' : EndLevelNode, b'\x93\x00' : SetGameLevel,
            b'\xBE\x00' : RunCinema, b'\x7E\x00' : SpoolIn,
            b'\x80\x00' : SpoolEnv, b'\x8E\x00' : SetObjFile,
            b'\xBD\x00' : SpoolCodeModule, b'\x84\x00' : BackgroundOff,
            b'\xFF\xFF' : Terminate, b'\x68\x00' : SetFoggingParam,
            b'\xA9\x00' : SetOTPushback2, b'\xA6\x00' : SetOTPushback,
            b'\xB4\x00' : SetSpideyCamValue, b'\xD9\x00' : AllowCamLOSCheck,
            b'\xCA\x00' : SetSkyColor, b'\xAB\x00' : BackgroundCreate,
            b'\x8D\x00' : SetBaddyVisibilityInBox, b'\x03\x00' : SendPulse}

    def Decode(self, index):
       #index of the op code to decode

        self.curInsn = index
        
        decoding = None
        while decoding == None:
            opCode = self.fileBuffer[self.curInsn : self.curInsn + 2]
            self.curInsn += 2
            decoding = self.opFuncList[opCode](self) 

        return
