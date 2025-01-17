﻿[ごちゃまぜドロップス](https://github.com/oov/aviutl_gcmzdrop)
の外部連携APIだけを独立した実行ファイルにすることを目的としたリポジトリです。  

## 注意事項  
動作にはVisual C++再頒布可能パッケージのVisualStudio2022 x64用のものが必要です。  
開発中なので頻繁に破壊的変更を行います。  

## 使用方法  
Visual C++再頒布可能パッケージのVisualStudio2022 x64をインストールしてください。  
https://learn.microsoft.com/ja-jp/cpp/windows/latest-supported-vc-redist?view=msvc-170  
<br>
ごちゃまぜドロップスv0.3.12以上を導入したAviUtlを起動した状態で、コマンドライン引数を指定し実行します。  
このとき、ファイルパスのディレクトリの区切りは\\\\\\\\としなければいけません。  
```
gcmzAPI.exe -f ファイルパス -l レイヤー -a 再生地点が移動するフレーム数
```

下記のコードでバージョンを取得できます。戻り値もバージョンになります。  
```
gcmzAPI.exe -v
```



## License  
### ごちゃまぜドロップス  
https://github.com/oov/aviutl_gcmzdrops  
The MIT License  

Copyright (c) 2016 oov

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.