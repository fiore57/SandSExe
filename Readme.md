# SandSExe

SandS(Space and Shift) を Windows で使うためのソフトです。

日本語入力時（直接入力モードでないとき）や、IME が ON のときのみ SandS を使う設定を用意しています。（ただし、この機能は [CorvusSKK](https://github.com/nathancorvussolis/corvusskk) でのみ正常に動作することを確認しています）

## 使用方法

### インストール

特別な操作は不要です。zip ファイルを展開すれば、すぐに使用できます。

このソフトは、管理者権限を要求します（管理者権限で動作するアプリ上で SandS を利用できるようにするため）。

そのため、SandSExe.exe の起動時に「この不明な発行元からのアプリがデバイスに変更を加えることを許可しますか？」などと聞かれます。このソフトを信用して使用する場合は、「はい」を押してください。

### アンインストール

このソフトはレジストリを使用しません。

- `config.ini`
- `SandS32.dll`
- `SandS64.dll`
- `SandSExe.exe`
- `SandSHidden32.exe`
- `SandSHidden64.exe`

を削除すれば OK です。

## 注意

- `config.ini`・`SandS32.dll`・`SandS64.dll`・`SandSExe.exe`・`SandSHidden32.exe`・`SandSHidden64.exe` は、同じフォルダにそのままの名前で置いてください
- このソフトを管理者権限なしで使用したい場合は、`SandSExe/CmakeLists.txt` の `level='requireAdministrator'` を `level='asInvoker'` に書き換えてビルドしてください
  - もちろん、この場合は管理者権限で動作しているソフトウェア上で SandS を利用することはできません。
- IME によっては、設定の「日本語入力時のみ」「IME 有効時のみ」が正常に動作しません（例：MS-IME・Google 日本語入力・[SKK 日本語入力 FEP](http://coexe.web.fc2.com/skkfep.html) など）。ご了承ください。
- Windows ストアアプリでは動作しません
  - どうしても使いたい場合は、`SandSExe/CMakeLists.txt` の `uiAccess=false` を `uiAccess=true` にしてビルドし、[https://qiita.com/QianJin2013/items/d743c3c9557203aadadd](https://qiita.com/QianJin2013/items/d743c3c9557203aadadd) あたりを参考に自己署名証明書を作成してください。（Windows ストアアプリでの動作確認はしていませんので、自己責任でお願いします）
- 製作者（fiore）は、このソフトウェアの使用によるいかなる損害も、その責任を一切負いません

## ライセンス

このソフトウェアは、MIT ライセンスのもとで公開されています。詳しくは、LICENSE.txt を見てください。
