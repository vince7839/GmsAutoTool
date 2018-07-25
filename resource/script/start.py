import pexpect
import sys

tool = sys.argv[1]
cmd = sys.argv[2]
child = pexpect.spawn(tool)
child.expect('ts-tf')
child.sendline(cmd)
child.interact()
