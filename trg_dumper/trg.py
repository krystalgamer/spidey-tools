import idaapi
import idautils
import idc

coisos = idautils.Strings()
SUPER_DUPER_OUTPUT = 'PATH_OF_THE_DUMP'

def get_string(ea):
    #fuck get_strlit_contents
    for s in coisos:
        if s.ea == ea:
            return str(s)
    return None

def main():
    if not idaapi.init_hexrays_plugin():
        return False

    f = idaapi.get_func(idaapi.get_screen_ea());
    if f is None:
        print "Please position the cursor within a function"
        return True

    cfunc = idaapi.decompile(f);
    if cfunc is None:
        print "Failed to decompile!"
        return True

    with open(SUPER_DUPER_OUTPUT, 'w') as f:
        sv = cfunc.get_pseudocode();
        for sline in sv:
            removido = idaapi.tag_remove(sline.line)
            if 'DoAssert' in removido:
                #This might be the ugliest code I've ever written, but hey it works ¯\_(ツ)_/¯
                removido = removido.lstrip()[9:].replace(');', '').split(',')
                removido = map(lambda x : x.lstrip(), removido)

                if len(removido[0]) > 4 and removido[0][0] == 'a':
                    removido = removido[0]
                else:
                    removido = removido[1]
                f.write(get_string(get_name_ea_simple(removido)) + '\n')
            elif 'case' in removido:
                #lstrip is bae and saviour of bad code
                f.write('Command ' + removido.lstrip()[4:].replace(':','').lstrip() + '\n')


    return True

if main():
    idaapi.term_hexrays_plugin();
