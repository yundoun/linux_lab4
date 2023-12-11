#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
  // GTK 초기화
  gtk_init(&argc, &argv);

  // 최상위 윈도우 생성
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  // 윈도우 제목 설정
  gtk_window_set_title(GTK_WINDOW(window), "GTK 예제 1");
  // 윈도우 테두리 너비 설정
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  // 윈도우 크기 설정
  gtk_widget_set_size_request(window, 300, 200);

  // 윈도우가 닫힐 때 프로그램 종료
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  // 레이블 위젯 생성 및 텍스트 설정
  GtkWidget *label = gtk_label_new("Hello, GTK!");
  // 레이블을 윈도우에 추가
  gtk_container_add(GTK_CONTAINER(window), label);

  // 윈도우와 그 내부 위젯들을 모두 표시
  gtk_widget_show_all(window);

  // GTK 이벤트 루프 실행
  gtk_main();

  return 0;
}
