import header_footer as hf
import math
import sys


def compress(f_name, wid, eps, header, footer, index_L, index_T, index_S):
    with open(f_name,'r') as rf:
        with open(f_name.replace('../raw/','../cmp/cmp_'),'w') as wf:
            wf.write(rf.readline())    # copy col index
            wf.write(rf.readline())    # copy obs name
            line_now = [float(element) for element in rf.readline().split()]
            num_col = len(line_now)
            ##########################################
            # get number of terms
            num_term, L_flag, T_flag = 0, 0, 0
            lines_data, num_member, start_index = {}, {}, {}
            index = 0
            while(True):
                lines_data[index] = line_now    # store line data
                if(line_now[index_L] == L_flag and line_now[index_T] == T_flag):  # count
                    num_member[num_term - 1] = num_member[num_term - 1] + 1
                else:  # update
                    num_term = num_term + 1
                    num_member[num_term - 1] = 1
                    start_index[num_term] = index
                    L_flag, T_flag = line_now[index_L], line_now[index_T]
                # get next line
                line_now = [float(element) for element in rf.readline().split()]
                if(line_now==[]): break
                index = index + 1
            ##########################################
            # compress data
            for i in range(num_term):
                context = ""
                si = start_index[i + 1]
                
                # skip header
                for j in range(header):
                    if(j == index_S):     # fill Seed with times
                        context = context + '\t' + str(str(num_member[i]) + 'x').ljust(wid)
                        continue
                    context = context + '\t' + str((lines_data[si][j])).ljust(wid)

                # only one 
                if(num_member[i] == 1):
                    for j in range(header, num_col):
                        context = context + '\t' + str(format(lines_data[si][j],".16f")).ljust(wid)

                # to mean
                else:
                    for j in range(int((num_col - footer - header) / 2)):
                        quan, err = 0.0, 0.0
                        quan_temp = {}
                        for k in range(num_member[i]):
                            err_ = lines_data[si + k][2 * j + header + 1]
                            if(err_ > eps):
                                err_ = 1.0 / (err_ * err_)
                            err = err + err_
                            quan = quan + lines_data[si + k][2 * j + header] * err_
                            quan_temp[k] = lines_data[si + k][2 * j + header]
                        if(abs(err > eps)):
                            quan = quan / err
                            err = math.sqrt(1.0 / err)
                        else:
                            quan = quan_temp[0]
                            err = eps
                        context = context + '\t' + str(format(quan,".16f")).ljust(wid) + '\t' + str(format(err,".16f")).ljust(wid)
                    # for footer
                    for j in range(num_col - footer, num_col):
                        quan = 0.0
                        for k in range(num_member[i]):
                            quan = quan + lines_data[si + k][j]
                        quan = quan / k
                        context = context + '\t' + str(format(quan,".16f")).ljust(wid)
                        
                wf.write(context + '\n')
            ##########################################


if __name__ == "__main__":

    wid = 16
    eps = 1E-13

    header = hf.header    # number of header
    footer = hf.footer    # number of footer

    L_index = hf.L_index
    beta_index = hf.beta_index
    seed_index = hf.seed_index

    f_name = sys.argv

    for i in range(len(f_name) - 1):
        compress(f_name[i + 1], wid, eps, header, footer, L_index - 1, beta_index - 1, seed_index - 1)
