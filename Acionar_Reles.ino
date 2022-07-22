#include <Ethernet.h>
#include <SPI.h>

// Complete com as informacoes da sua rede
byte mac[] =
{
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(192, 168, 25, 100);
int saidaSensor = 4;
// Inicializa o servidor Ethernet com o IP e a porta 84
EthernetServer server(84);
String readString;

void setup()
{
  // Comeca a conexao com o servidor Ethernet
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(saidaSensor, OUTPUT);
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client)
  {
    Serial.println("Got a client");
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        //read char by char HTTP request
        if (readString.length() < 100)
        {
          //store characters to string
          readString += c;
        }
        if (c == '\n')
        {
          //Imprimne a string lida na serial. Pode comentar se quiser
          Serial.println("readString: ");
          Serial.println(readString);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Acione a sua luminaria pela net</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<H1>Acione os pinos do Arduino por essa pagina</H1>");
          client.println("<H2>Exemplo com pino 4</H2>");
          client.println("<FORM ACTION=\"http://192.168.25.100:84\"method=get >");
          client.println("<a href=\"/?liga\">LIGA</a>");
          client.println("<a href=\"/?desliga\">DESLIGA</a>");
          client.println("</FORM>");
          client.println("<BR>");
          client.println("</BODY>");
          client.println("</HTML>");
          delay(1);
          client.stop();
          if (readString.indexOf("liga") > 0)
          {
            digitalWrite(4, HIGH);
            Serial.println("Led On");
          }
          if (readString.indexOf("desliga") > 0)
          {
            digitalWrite(4, LOW);
            Serial.println("Led Off");
          }
          //Limpa a String
          readString = "";
        }
      }
    }
  }
}