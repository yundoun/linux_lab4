#include <gtk/gtk.h>

GtkWidget *entry;                            // 숫자를 입력하는 Entry 위젯
gdouble num1 = 0.0, num2 = 0.0;              // 입력된 숫자 변수
gboolean num1_set = FALSE, num2_set = FALSE; // 입력된 숫자 상태
gchar operation = '\0';                      // 현재 선택된 연산자

// 숫자 버튼 또는 연산자 버튼이 클릭되었을 때 호출되는 콜백 함수
void on_button_clicked(GtkWidget *widget, gpointer data)
{
  const gchar *text = gtk_button_get_label(GTK_BUTTON(widget)); // 버튼의 레이블 텍스트 가져오기
  gchar *current_text = gtk_entry_get_text(GTK_ENTRY(entry));   // Entry 위젯의 현재 텍스트 가져오기

  if (g_ascii_isdigit(text[0])) // 만약 클릭된 버튼이 숫자 버튼인 경우
  {
    if (current_text[0] == '0' && current_text[1] == '\0') // 현재 텍스트가 "0"일 때
    {
      gtk_entry_set_text(GTK_ENTRY(entry), text); // 클릭된 숫자로 텍스트 설정
    }
    else // 현재 텍스트가 "0"이 아닐 때
    {
      gchar *new_text = g_strdup_printf("%s%s", current_text, text); // 현재 텍스트와 클릭된 숫자를 합치기
      gtk_entry_set_text(GTK_ENTRY(entry), new_text);                // 합친 텍스트로 업데이트
      g_free(new_text);                                              // 메모리 해제
    }
  }
  else if (text[0] == '+' || text[0] == '-' || text[0] == '*' || text[0] == '/') // 만약 클릭된 버튼이 연산자 버튼인 경우
  {
    if (!num1_set) // 첫 번째 숫자가 아직 설정되지 않은 경우
    {
      num1 = g_ascii_strtod(current_text, NULL); // 현재 텍스트를 실수로 변환하여 첫 번째 숫자로 설정
      num1_set = TRUE;                           // 첫 번째 숫자 설정 상태로 변경
      operation = text[0];                       // 현재 선택된 연산자 저장
      gtk_entry_set_text(GTK_ENTRY(entry), "");  // Entry 위젯 텍스트 초기화
    }
  }
  else if (text[0] == '=') // "=" 버튼이 클릭된 경우
  {
    if (num1_set) // 첫 번째 숫자가 설정되어 있는 경우
    {
      num2 = g_ascii_strtod(current_text, NULL); // 현재 텍스트를 실수로 변환하여 두 번째 숫자로 설정
      gdouble result = 0.0;                      // 결과를 저장할 변수 초기화

      switch (operation) // 현재 선택된 연산자에 따라 연산 수행
      {
      case '+':
        result = num1 + num2;
        break;
      case '-':
        result = num1 - num2;
        break;
      case '*':
        result = num1 * num2;
        break;
      case '/':
        if (num2 != 0)
        {
          result = num1 / num2;
        }
        else
        {
          gtk_entry_set_text(GTK_ENTRY(entry), "Error: Division by zero"); // 0으로 나누는 에러 처리
          num1_set = FALSE;                                                // 상태 초기화
          num2_set = FALSE;
          return; // 함수 종료
        }
        break;
      }

      gchar *result_text = g_strdup_printf("%.2f", result); // 결과를 문자열로 변환
      gtk_entry_set_text(GTK_ENTRY(entry), result_text);    // 결과를 Entry 위젯에 표시
      g_free(result_text);                                  // 메모리 해제
      num1_set = FALSE;                                     // 상태 초기화
      num2_set = FALSE;
    }
  }
  else if (text[0] == 'C') // "C" 버튼이 클릭된 경우 (초기화)
  {
    gtk_entry_set_text(GTK_ENTRY(entry), ""); // Entry 위젯 텍스트 초기화
    num1_set = FALSE;                         // 상태 초기화
    num2_set = FALSE;
  }
}

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv); // GTK 초기화

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);   // 최상위 윈도우 생성
  gtk_window_set_title(GTK_WINDOW(window), "Calculator");    // 윈도우 제목 설정
  gtk_container_set_border_width(GTK_CONTAINER(window), 10); // 테두리 너비 설정
  gtk_widget_set_size_request(window, 300, 400);             // 윈도우 크기 설정

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // 윈도우 닫기 버튼 동작 설정

  GtkWidget *grid = gtk_grid_new();               // 그리드 컨테이너 생성
  gtk_container_add(GTK_CONTAINER(window), grid); // 윈도우에 그리드 추가

  entry = gtk_entry_new();                            // 숫자 입력을 받을 Entry 위젯 생성
  gtk_widget_set_hexpand(entry, TRUE);                // Entry 위젯이 가로로 확장되도록 설정
  gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1); // 그리드에 Entry 위젯 추가

  // 숫자와 연산자 버튼 레이아웃을 정의
  gchar *buttons[] = {
      "7", "8", "9", "/",
      "4", "5", "6", "*",
      "1", "2", "3", "-",
      "C", "0", "=", "+"};

  GtkWidget *button;
  int row = 1, col = 0;

  for (int i = 0; i < 16; i++)
  {
    button = gtk_button_new_with_label(buttons[i]);          // 버튼 생성
    gtk_widget_set_hexpand(button, TRUE);                    // 버튼이 가로로 확장되도록 설정
    gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1); // 그리드에 버튼 추가

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL); // 버튼 클릭 시 콜백 함수 연결

    col++;
    if (col > 3)
    {
      col = 0;
      row++;
    }
  }

  gtk_widget_show_all(window); // 윈도우와 그 내부 위젯들을 모두 표시

  gtk_main(); // GTK 이벤트 루프 실행

  return 0;
}
