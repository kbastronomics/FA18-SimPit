Import("env")
import shutil
from os import listdir
from os.path import isfile, join

def post_program_action(source, target, env):
    src_dir = env["PROJECT_SRC_DIR"]
    print(">> Project Source Dir",src_dir)
    onlyfiles = [f for f in listdir(src_dir) if isfile(join(src_dir, f))]
    main_file = onlyfiles[0]
    print(">> Main Source File",main_file)
    ino_file = main_file.replace(".cpp", ".ino")
    ino_path = src_dir + '\\' + '..\Arduino' + '\\'
    shutil.copyfile(src_dir + '\\' + main_file, ino_path + ino_file)

env.AddPostAction("$PROGPATH", post_program_action)
# env.AddPreAction("$buildprog", pre_program_action)