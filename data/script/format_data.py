import header_footer as hf
import os
import sys

def format_data(r_name, w_name, wid, bas_wid):

    index = 0
    context_1 = "#"         # obs index list
    context_2 = "#"         # obs name list
    context_3 = " "         # obs value list

    # load data
    with open(r_name,'r') as rf:
        # get first block
        while True:
            line_now = rf.readline().split()
            if any('======' in item for item in line_now):
                break
        
        # write basic info for simulation
        ##########################################################
        while True:
            line_now = rf.readline().split()
            if any('======' in item for item in line_now):
                break
            check_times = int(len(line_now)/bas_wid)
            for i in range(check_times):
                index = index + 1
                context_1 = context_1 + '\t' + str(index).ljust(wid)
                context_2 = context_2 + '\t' + str(line_now[3 * i]).ljust(wid)
                context_3 = context_3 + '\t' + str(line_now[3 * i + 2]).ljust(wid)

        # write obs info for simulation
        ##########################################################
        line_now = rf.readline().split()
        if any('Average' in item for item in line_now):
            index = index + 1
            context_1 = context_1 + '\t' + str(index).ljust(wid)
            context_2 = context_2 + '\t' + str(line_now[-3]).ljust(wid)
            context_3 = context_3 + '\t' + str(line_now[-1]).ljust(wid)
        else:
            os.system('rm -f '+ w_name)
            exit()
        ##########################################################
        while True:
            line_now = rf.readline().split()
            if any('NperBin' in item for item in line_now):
                break
            # value
            index = index + 1
            context_1 = context_1 + '\t' + str(index).ljust(wid)
            context_2 = context_2 + '\t' + str(line_now[1]).ljust(wid)
            context_3 = context_3 + '\t' + str(line_now[3]).ljust(wid)
            # error
            index = index + 1
            context_1 = context_1 + '\t' + str(index).ljust(wid)
            context_2 = context_2 + '\t' + str(line_now[1] + '_err').ljust(wid)
            context_3 = context_3 + '\t' + str(line_now[4]).ljust(wid)
            # print(line_now)
        ##########################################################

    # format data
    with open(w_name,'w') as wf:
        wf.write(context_1 + '\n')
        wf.write(context_2 + '\n')
        context_3 = context_3.replace('%',' ')  # clear % char
        wf.write(context_3 + '\n')


if __name__ == "__main__":
    wid = 16
    bas_wid = 3

    r_name = sys.argv[1]
    w_name = sys.argv[1].replace(hf.format_file, hf.formated_file)

    format_data(r_name, w_name, wid, bas_wid)