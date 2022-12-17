from genericpath import exists
import os
import sys
import shutil
import subprocess
import time

args = sys.argv

eset_app = "ESET Endpoint Security.app"
eset_path = f"/Applications/{eset_app}"
copy_path = os.path.expanduser("/tmp/killer/")

try:
    copy_path = os.path.expanduser(args[1])
    if copy_path[-1] != "/":
        copy_path += "/"
except:
    print("コピー先のパスが指定されませんでした")

print(f"コピー先パス: {copy_path}")

if not os.path.exists(f"{eset_path}"):
    print("ESETが見つかりません！！")
    exit(0)

print("ESETをコピーします")

if not os.path.exists(f"{copy_path}"):
    print("コピー先パスが存在しないため、作成します")
    os.makedirs(copy_path, exist_ok=True)

if not os.path.exists(f"{copy_path}{eset_app}"):
    shutil.copytree(eset_path, f"{copy_path}{eset_app}")
else:
    print("ESETは既にコピー済み")

lproj_path = f"{copy_path}{eset_app}/Contents/Resources/ja.lproj/Localizable.strings"


with open(lproj_path, encoding="utf-16") as f:
    data_lines = f.read()

# 文字列置換
data_lines = data_lines.replace('COMMAND=\\\"PRESENTATION-MODE\\\"', "COMMAND=\\\"RESTART-SERVICE\\\"")
data_lines = data_lines.replace("プレゼンテーションモードを無効にする", "ESETをリスタート")

# 同じファイル名で保存
with open(lproj_path, mode="w", encoding="utf-16") as f:
    f.write(data_lines)

print("翻訳ファイル編集完了")

esets_gui = f"'{copy_path}{eset_app}/Contents/MacOS/esets_gui'"

while(True):
    print("ESETを起動します")
    proc = subprocess.call(f"{esets_gui}" , shell=True)
    print("ESETが再起動されました")
    print("2秒待機します")
    time.sleep(2)
