#!/usr/bin/python

import sys
import os

filename = sys.argv[1]

outputfile_name = sys.argv[2]
os.system("cp library.c " + outputfile_name + ".c")
os.system("cp mac.h " + outputfile_name + ".h")

outputfile_name = outputfile_name + ".c"
outfile = open(outputfile_name, "a")

sendpacket_template_start = r"""
static void
send_packet(mac_callback_t sent, void *ptr)
{
"""

sendpacket_template_end = r"""
}
"""

parser_state = "start"

for line in open(filename, "r").readlines():
    line = line.replace("\n", "")
    parenthesis_start = line.find("(")
    parenthesis_end = line.find(")")
    function_name = line[:parenthesis_start]
    arguments = line[parenthesis_start + 1 : parenthesis_end].split(",")

    
    if function_name == "wait_for_packet":
        parser_state = "wait_for_packet"    
        outfile.write(sendpacket_template_start)

    if parser_state == "wait_for_packet" and function_name == "randombackoff_blocking":
        outfile.write("randombackoff_blocking(" + arguments[0] + ");")
        outfile.write("\n")
        
    if parser_state == "wait_for_packet" and function_name == "send_frame":
        outfile.write("send_frame(sent, ptr);")
        outfile.write("\n")

    if parser_state == "wait_for_packet" and function_name == "end":
        outfile.write(sendpacket_template_end)
        
outfile.close()