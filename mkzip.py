#!/usr/bin/env python3

import os,zipfile,tkinter.filedialog,os.path


goodFiles = set( ["cpp","c","h","txt","vs","fs","gs","cs","cc",".png",".jpg"] )
forbiddenDirs = set( ["x86","x64",".vs","bin","obj","debug","release","__macosx"] )
#https://stackoverflow.com/questions/1732813/best-practices-for-subversion-and-visual-studio-projects
#https://stackoverflow.com/questions/3922660/which-visual-c-file-types-should-be-committed-to-version-control
forbiddenFiles = set( ["pdb","suo","user","glb","webinfo","dll","exe","ncb","obj",
    "projdata","positions","plg","opt","ilk","pch","idb","clw","cache","snk",
    "tlb","lib","aps","ipch","log","manifest","res","sdf","tlog","mdp","cpl",
    "awk","exp","map","scc","bsc","sbr","zip","7z","rar","o"] )
    
def main():
    root = tkinter.filedialog.askdirectory(mustexist=True,title="Lab 2 Bumpmap")
    if not root:
        return
        
    root = os.path.realpath(root)
    
    with zipfile.ZipFile( os.path.join(root,"lab.zip"),"w" ) as zfp:
        for dirpath,dirnames,filenames in os.walk(root):
            i=0
            while i < len(dirnames):
                dname = dirnames[i]
                if dname.lower() in forbiddenDirs:
                    del dirnames[i]
                else:
                    i+=1
            for fn in filenames:
                tmp = fn.split(".")
                sfx = tmp[-1].lower()
                if sfx in forbiddenFiles:
                    continue
                
                p = os.path.join(dirpath,fn)
                p = os.path.realpath(p)
                if p.startswith(root):
                    archivePath = p[len(root):]
                    if archivePath.startswith("/") or archivePath.startswith("\\"):
                        archivePath = archivePath[1:]
                else:
                    archivePath = None
                
                print("Adding",p,"as",archivePath)
                zfp.write( p, arcname=archivePath )
                
    print("Done.")
                    
main()
