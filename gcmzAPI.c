#include <stdint.h>
#include <stdio.h>

#define UNICODE
#include <Windows.h>

struct GCMZDropsData {
  uint32_t Window;
  int32_t Width;
  int32_t Height;
  int32_t VideoRate;
  int32_t VideoScale;
  int32_t AudioRate;
  int32_t AudioCh;
  int32_t GCMZAPIVer;
  wchar_t ProjectPath[MAX_PATH];
  uint32_t Flags; // GCMZAPIVer が 2 以上なら存在する
};

int main(int argc, char *argv[]) {

    char addfile[4096] = "n";
    int layer = -1;
    int frameAdvance = 0;

    char lpString[8192];

        int i = 1;
    while (argv[i] != NULL) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'f':   //ファイルパス
                i++;
                strcpy(addfile, argv[i]);
                break;

            case 'l':   //レイヤー
                i++;
                layer = atoi(argv[i]);
                break;

            case 'a':   //カーソル移動
                i++;
                frameAdvance = atoi(argv[i]);
                break;

            default:
                break;
          
            }
        }
    i++;
    }

    if(addfile[0] == 'n') {
        return 1;
    }

    printf("addfile: %s\n", addfile);
    


  HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("GCMZDropsMutex"));
  if (hMutex == NULL) {
    printf("OpenMutex に失敗しました。\n");
    return 122;
  }

  HANDLE hFMO = OpenFileMapping(FILE_MAP_READ, FALSE, TEXT("GCMZDrops"));
  if (hFMO == NULL) {
    printf("OpenFileMapping に失敗しました。\n");
    return 123;
  }

  struct GCMZDropsData *p = MapViewOfFile(hFMO, FILE_MAP_READ, 0, 0, 0);
  if (p == NULL) {
    printf("MapViewOfFile に失敗しました。\n");
    goto CloseFMO;
  }

  WaitForSingleObject(hMutex, INFINITE);

  HWND targetWnd = (HWND)(ULONG_PTR)p->Window;
  if (targetWnd == NULL) {
    printf("対象ウィンドウの取得に失敗しました。\n");
    goto Unmap;
  }

  if (p->Width == 0) {
    printf("プロジェクトが開かれていません。\n");
    goto Unmap;
  }

  // v0.3.23 以降なら 2
  // v0.3.12 以降なら 1
  printf("GCMZAPIVer: %d\n", p->GCMZAPIVer);

  printf("ProjectPath(%d): %ls\n", (int)wcslen(p->ProjectPath), p->ProjectPath);
  printf("Window: %d\n", p->Window);
  printf("Width: %d\n", p->Width);
  printf("Height: %d\n", p->Height);
  printf("VideoRate: %d\n", p->VideoRate);
  printf("VideoScale: %d\n", p->VideoScale);
  printf("AudioRate: %d\n", p->AudioRate);
  printf("AudioCh: %d\n", p->AudioCh);

  // GCMZAPIVer が 2 以上なら Flags が存在する
  if (p->GCMZAPIVer >= 2) {
    printf("Flags: %d\n", (int)p->Flags);
    if (p->Flags & 1) {
      // 英語化パッチが当たっている拡張編集だった
      printf("  English Patched\n");
    }
    if (p->Flags & 2) {
      // 中国語簡体字パッチが当たっている拡張編集だった
      printf("  Simplified Chinese Patched\n");
    }
  }

  // GCMZAPIVer が 0 のときは対応しない　※API 仕様が異なります
  if (p->GCMZAPIVer == 0) {
    printf("GCMZDrops too old, please update to v0.3.12 or later.");
    goto Unmap;
  }

  HWND myWnd = GetConsoleWindow();

  COPYDATASTRUCT cds;

  // 必ず 1 を指定してください。
  cds.dwData = 1;

  // JSON を UTF-8 エンコーディングで渡します。
  // layer:
  //   ドロップするレイヤーを決めます。
  //   指定を省略することはできません。
  //   -1 ～ -100
  //       拡張編集上での現在の表示位置からの相対位置へ挿入
  //       例: 縦スクロールによって一番上に見えるレイヤーが Layer 3 のとき、-1 を指定すると Layer 3、-2 を指定すると Layer 4 へ挿入
  //    1 ～  100
  //       スクロール位置に関わらず指定したレイヤー番号へ挿入
  // frameAdvance:
  //   ファイルのドロップした後、指定されたフレーム数だけカーソルを先に進めます。
  //   進める必要がない場合は省略可能です。
  // files:
  //   投げ込むファイルへのフルパスを配列で渡します。
  //   ファイル名は UTF-8 にする必要がありますが、拡張編集の仕様上 ShiftJIS の範囲内の文字しか扱えません。

    sprintf(lpString, "{\"layer\":%d,\"frameAdvance\":%d,\"files\":[\"%s\"]}", layer, frameAdvance, addfile);
    cds.lpData = lpString;

  cds.cbData = strlen(cds.lpData);

  // API を呼び出します
  //   cds.dwData の値が間違っている場合や JSON がおかしい場合など、
  //   API としての送信フォーマットに問題がある場合には OutputDebugString でエラーメッセージを出力します。
  //   ドロップするファイルが見つからないなど、ファイルの内容に問題がある場合はメッセージボックスで表示します。
  SendMessage(targetWnd, WM_COPYDATA, (WPARAM)myWnd, (LPARAM)&cds);

Unmap:
  ReleaseMutex(hMutex);

  if (UnmapViewOfFile(p) == 0) {
    printf("UnmapViewOfVile に失敗しました。\n");
    goto CloseFMO;
  }

CloseFMO:
  CloseHandle(hFMO);
  CloseHandle(hMutex);
  return 0;
}