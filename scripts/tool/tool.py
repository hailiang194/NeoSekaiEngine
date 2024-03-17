import argparse
import urllib.request
import json
import sys
import os
import uuid
import tempfile
import shutil
import zipfile
import platform

def get_all_version():
    result = []
    req = urllib.request.Request("https://api.github.com/repos/hailiang194/NeoSekaiEngine/tags")
    with urllib.request.urlopen(req) as res:
        # data = json.load(res.read().decode(res.info().get_param('charset') or 'utf-8'))
        # rawData = res.read().decode("utf-8")
        try:
            tags = json.load(res)
            result.append("unstable")
            result.extend([tag["name"] for tag in tags])
        except:
            pass
    
    return result

def query_yes_no(question, default="yes"):
    """Ask a yes/no question via raw_input() and return their answer.

    "question" is a string that is presented to the user.
    "default" is the presumed answer if the user just hits <Enter>.
            It must be "yes" (the default), "no" or None (meaning
            an answer is required of the user).

    The "answer" return value is True for "yes" or False for "no".
    """
    valid = {"yes": True, "y": True, "ye": True, "no": False, "n": False}
    if default is None:
        prompt = " [y/n] "
    elif default == "yes":
        prompt = " [Y/n] "
    elif default == "no":
        prompt = " [y/N] "
    else:
        raise ValueError("invalid default answer: '%s'" % default)

    while True:
        sys.stdout.write(question + prompt)
        choice = input().lower()
        if default is not None and choice == "":
            return valid[default]
        elif choice in valid:
            return valid[choice]
        else:
            sys.stdout.write("Please respond with 'yes' or 'no' " "(or 'y' or 'n').\n")

class ShowAllVersionAction(argparse.Action):
    def __init__(self,
                 option_strings,
                 version=None,
                 dest=argparse.SUPPRESS,
                 default= argparse.SUPPRESS,
                 help="show program's version number and exit"):
        super(ShowAllVersionAction, self).__init__(
            option_strings=option_strings,
            dest=dest,
            default=default,
            nargs=0,
            help=help)


    def __call__(self, parser, namespace, values, option_string = None):
        print("Show all versions")
        print("Make sure your Internet connection")

        versions = get_all_version()
        for version in versions:
            print("\t{}".format(version))
        parser.exit()

def setup_project(project_name, version, is_yes, verbose, project_path):
    # print(tempfile.gettempdir())
    # print("{} {} {} {}".format(project_name, version, is_yes, verbose))
    if not os.path.isdir(project_path):
        print("Invalid project path")
        exit(-1)

    project_source_path = os.path.join(project_path, project_name)

    if os.path.isdir(project_source_path):
        if not is_yes and not query_yes_no("Folder has been exist, delete and recreate"):
            print("Invalid path")
            exit(-1)
        
        shutil.rmtree(project_source_path)
    
    os.mkdir(project_source_path)
    
    download_url_header = "https://github.com/hailiang194/NeoSekaiEngine/archive/refs/"
    download_surfix = "heads/development.zip" if version == "unstable" else "tags/{}.zip".format(version)

    url = download_url_header + download_surfix
    if verbose:
        print("Downloading NeoSekaiEngine, version={}...".format(version), end="")

    engine_zip_path = os.path.join(tempfile.gettempdir(), "NeoSekaiEngine_{}_{}.zip".format(version, uuid.uuid1()))

    _, status = urllib.request.urlretrieve(url, engine_zip_path)

    if os.path.exists(engine_zip_path):
        if verbose:
            print("Done")
            print("Engine zip path={}".format(engine_zip_path))
    else:
        if verbose:
            print("Failed")
            print("Response status:\n{}".format(status))
        
        print("Failed to fetch engine")
        exit(-1)

    with zipfile.ZipFile(engine_zip_path, "r") as file:
        if verbose:
            file.printdir()

        for info in file.infolist():
            
            desc_path = os.path.join(project_source_path, os.path.join("NeoSekaiEngine", os.sep.join(info.filename.split("/")[1:])))
            if info.is_dir():
                os.mkdir(desc_path)
                continue

            with file.open(info) as fin, open(desc_path, "wb") as fout:
                while True:
                    buf = fin.read(16 * 1024)
                    if not buf:
                        break

                    fout.write(buf)

    if verbose:
        print("Delete {}".format(engine_zip_path))
    os.remove(engine_zip_path)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="NeoSekaiEngine setup project")
    parser.add_argument("--engine_versions", help="Show all engine versions", action=ShowAllVersionAction)
    parser.add_argument("-y", "--yes", action='store_true', help="Yes to all for every prompt")
    parser.add_argument("-v", "--verbose", action='store_true', help="Show verbose")
    parser.add_argument("-p", "--path", help="Project path", default=".")
    parser.add_argument("project_name", help="Name of project")
    parser.add_argument("engine_version", help="set version of engine")


    args = parser.parse_args()

    if args.verbose:
        print("Fetching engine version...", end="")

    versions = get_all_version()
    
    if len(versions) == 0:
        print("Unable to fetch engine version")
        exit(-1)

    if args.verbose:
        print("Done")
    
    ver = "unstable"
    if args.engine_version == "latest":
        ver = versions[1]
    elif  not args.engine_version in versions:
        print("Unvailable version")
        print("Avavaible versions:")
        for version in versions:
            print("\t{}".format(version))
        exit(-1)

    setup_project(args.project_name, ver, args.yes, args.verbose, args.path)
    