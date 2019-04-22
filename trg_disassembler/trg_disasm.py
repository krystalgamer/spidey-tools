from struct import unpack
import sys
import os

class Trg(object):
    def __init__(self, filename):
        self.trg_file = None
        self.trg_file_name = filename
        self.num_nodes = 0
        self.file_pointer = 0
        self.cur_node = 0
        self.re_start_death = False

    def load_trg(self):
        try:
            with open(self.trg_file_name, mode='rb') as f:
                self.trg_file = f.read()
        except IOError:
            return False
        return True

    possible_sizes = {1:'B', 2:'H', 4:'I'}
    word = 2
    byte = 1
    dword = 4
    def dereference(self, address, size):
        return unpack(self.possible_sizes[size], self.trg_file[address:address+size])[0]

    def dererence_string(self, address):
        final = ''
        cur_index = 0
        while True:
            cur_char = self.dereference(address+cur_index, self.byte)
            if cur_char == 0x00:
                break
            final += chr(cur_char)
            cur_index+=1

        return final

    def check_trg_headers(self):

        magic = self.dereference(0,self.dword)
        if  magic != 0x4752545F:
            print('TRG magic number not found {}'.format(magic))
            return False

        version = self.dereference(4, self.dword)
        if (version & 0xFFFF) != 2:
            print('TRG version is wrong')
            return False

        if (version & 0xFFFF0000) != 0x10000:
            print('Not a spidey TRG')
            return False

        print('Headers are correct')
        return True

    def get_num_nodes(self):
        self.num_nodes = self.dereference(8, self.dword)
        print('There are {} nodes'.format(self.num_nodes))

    def ensure_terminator_node(self):
        terminator_node_index = 0xC + 4 * self.num_nodes - 4
        terminator_node = self.dereference(terminator_node_index, self.dword)

        if self.dereference(terminator_node, self.word) != 0xFF:
            print('Missing terminator node')
            return False
        return True

    def get_restart_points(self):
        num_points = 0
        for i in range(self.num_nodes):
            cur_node_index = 0xC + 4*i
            address_of_node = self.dereference(cur_node_index, self.dword)
            if self.dereference(address_of_node, self.word) == 8:
                print('Found one on node {}'.format(i))
                #Not sure what it really returns
                trg_position_result = self.get_position(address_of_node)
                print(self.dererence_string(trg_position_result+6))
                num_points +=1
        return num_points

    def get_position(self, address):
        #TODO look better into it, it's really weird
        if self.dereference(address, self.word) == 8:
            new_offset = (self.dereference(address+2, self.word)+3)*2
            return (((address+new_offset)+1)&0xFFFFFFFC)+12
        else:
            raise ValueError('Didnt add all the opcodes yet')

    def run_autoexec(self):
        for i in range(self.num_nodes):
            cur_node_index = 0xC + 4*i
            address_of_node = self.dereference(cur_node_index, self.dword)
            if self.dereference(address_of_node, self.word) == 4:
                print('Found one on node {}'.format(i))
                self.file_pointer = address_of_node+2
                self.cur_node = i
                self.run_node()

    def SetRestart(self):
        #TODO 0xB0 opcode is kinda funky
        restart_point = self.dererence_string(self.file_pointer)
        if restart_point == 're_start_death':
            self.re_start_death = True
        print('SetRestart = {}'.format(restart_point))
        self.file_pointer += len(restart_point)
        if self.file_pointer & 1 == 1:
            self.file_pointer += 1
        else:
            self.file_pointer += 2


    def EndLevelNode(self):
        #COMPLETED
        print('EndLevelNode = {}'.format(self.dereference(self.file_pointer, self.word)))
        self.file_pointer += 2

    def SetGameLevel(self):
        #COMPLETED
        print('SetGameLevel ({})'.format(self.dereference(self.file_pointer, self.word)))
        self.file_pointer += 2

    def RunCinema(self):
        #COMPLETED
        cur_node_index = 0xC + 4*self.cur_node
        address_of_node = self.dereference(cur_node_index, self.dword)
        node_start = self.dereference(address_of_node, self.word)  
        if (node_start == 4 or node_start == 15) and self.re_start_death is True:
            print('RunCinema(ignored)')
        else:
            print('RunCinema({})'.format(self.dereference(self.file_pointer, self.word)))
        self.file_pointer += 2
    
    def SpoolIn(self):
        #TODO Skiplib.txt
        file_name = self.dererence_string(self.file_pointer)
        print('SpoolIn({}) -- MISSING SKIPLIB part'.format(file_name))
        self.file_pointer += len(file_name)
        if self.file_pointer & 1 == 1:
            self.file_pointer += 1
        else:
            self.file_pointer += 2

    def SpoolEnv(self):
        #COMPLETED
        file_name = self.dererence_string(self.file_pointer)
        print('SpoolEnv({})'.format(file_name))
        self.file_pointer += len(file_name)
        if self.file_pointer & 1 == 1:
            self.file_pointer += 1
        else:
            self.file_pointer += 2

    def SetObjFile(self):
        #COMPLETED
        file_name = self.dererence_string(self.file_pointer)
        print('SetObjFile({})'.format(file_name))
        self.file_pointer += len(file_name)
        if self.file_pointer & 1 == 1:
            self.file_pointer += 1
        else:
            self.file_pointer += 2

    def SpoolCodeModule(self):
        #COMPLETED
        file_name = self.dererence_string(self.file_pointer)
        print('SpoolCodeModule({})'.format(file_name))
        self.file_pointer += len(file_name)
        if self.file_pointer & 1 == 1:
            self.file_pointer += 1
        else:
            self.file_pointer += 2

    def BackgroundOff(self):
        #COMPLETED
        print('BackgroundOff {}'.format(self.dereference(self.file_pointer, self.word)))
        self.file_pointer += 2

    def SetSkyColor(self):
        #COMPLETED
        color = hex(self.dereference(self.file_pointer, self.word)<<16 + self.dereference(self.file_pointer+2, self.word))
        print('SetSkyColor {}'.format(color))
        self.file_pointer += 4

    def SetFadeColor(self):
        #COMPLETED
        color = hex(self.dereference(self.file_pointer, self.word)<<16 + self.dereference(self.file_pointer+2, self.word))
        print('SetFadeColor {}'.format(color))
        self.file_pointer += 4

    def SetDualBufferSize(self):
        #COMPLETED
        print('SetDualBufferSize(unimplemented)')
        self.file_pointer += 2

    def SetSuspendDistance(self):
        #COMPLETED
        print('SetSuspendDistance({})'.format(self.dereference(self.file_pointer, self.word)))
        self.file_pointer += 2

    trg_function_table = { 0x8C: SetRestart, 0x12E: EndLevelNode, 0x93: SetGameLevel,
            0xBE: RunCinema, 0x7E: SpoolIn, 0x80: SpoolEnv, 0x8E: SetObjFile, 0xBD: SpoolCodeModule,
            0x84: BackgroundOff, 0xCA: SetSkyColor, 0xC8: SetFadeColor, 0x97: SetDualBufferSize,
            0xAA: SetSuspendDistance}

    def run_node(self):
        cur_opcode = 0
        try:
            while True:
                cur_opcode = self.dereference(self.file_pointer, self.word)
                if cur_opcode == 0xFFFF:
                    print('Reached terminator node')
                    break
                self.file_pointer += 2
                self.trg_function_table[cur_opcode](self)
        except KeyError:
            print('Need to add key {} at position {}'.format(hex(cur_opcode), self.file_pointer))
            raise KeyError('LMAO')



def main():
    if len(sys.argv) != 2:
        print('You must pass one argument')
        return
    files = os.listdir('.')
    files = list(filter(lambda x: '.py' not in x, files))
    if len(files) != 68:
        print('We got a problem')
        return
    
    valid_files = 0
    coisos = []
    for entry in files:
        try:
            #trg = Trg(sys.argv[1])
            print('Looking at {}'.format(entry))
            trg = Trg(entry)
            trg.load_trg()
            if trg.check_trg_headers() != True:
                return

            trg.get_num_nodes() 
            trg.ensure_terminator_node()
            print('There are {} restart points'.format(trg.get_restart_points()))

            print('Running AUTOEXEC')
            trg.run_autoexec()
            valid_files += 1
        except KeyError:
            coisos.append(entry)
            pass


    print('{}/68 pass the disassembler!'.format(valid_files))
    print(coisos)
    print('Everything is fine until here')

if __name__ == '__main__':
    main()
