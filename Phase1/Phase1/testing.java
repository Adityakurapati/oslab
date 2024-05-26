import java.io.*;
class Main{
        public static void main(String arg[]){
                OS ubantu os = new  OS("input.txt","output.txt");
        }
}
class Os{
        char memory[][]=new char[100][4];
        char buffer[] = new char[40];
        char R[]=new char[4];
        char IR[]=new char[4];
        int IC;
        int T;
        int SI;

        int memory_used;
        String input_file;
        String output_file;
        FileReader input;
        BufferedReader fread;
        FileWriter output;
        BufferedWriter fwrite;

        puplic OS(String ifile,String ofile){
                this.input_file = ifile;
                this.output_file = ofile;
                this.SI=0;
                try{
                        this.input = FileReader(input_file);
                        this.fread = BufferedReader(input);
                        this.output = new FileWriter(output_file);
                        this.fwrite=BufferedWriter(output);
                }catch(IOException e){
                        e.printStackTrace();
                }
        }
        void init(){
                memory_used =0;
                memory =null;
                memory = new char[100][4];
                T=0;
                IC=0;
        }
        public void LOAD(){
                int flag = 0;
                String line;
                try{
                        while((line=fread.readLine())!=null){
                                buffer = line.toCharArray();
                                if(buffer.length==0){
                                        continue;
                                }
                                if(buffer[0]=='$'&&buffer[1]=='A'&&buffer[2]=='M'&&buffer[3]=='J'){
                                        System.out.println("Program Card Detected ...");
                                        init();
                                        continue;
                                }else if(buffer[0]=='$'&&buffer[1]=='D'&buffer[2]=='T'&&buffer[3]=='A'){
                                        System.out.println("Data Card Detected ... ");
                                        execute();
                                        flag=2;
                                        continue;
                                }
                                else if(buffer[0]=='$'&&buffer[1]=='E'&&buffer[2]=="N"&&buffer[3]=='D'){
                                        System.out.println("End card Detected ...");
                                        fwrite.write("\n\n\n");
                                        continue;
                                }if(memory_used == 100){
                                        System.out.println("Abort Due TO Exeeded Memory ");
                                }

                                for(int i=0;i<line.length;){
                                        memory[memory_used][i%4]=buffer[i];
                                        i++;
                                        if(i%4==0){
                                                memory_used++;
                                        }
                                }                         write.close();       
                        }
                }catch(IOException e){e.printStackTrace();}
        }
}
void execute(){
        while(1<2){
                if(IC==100){
                        break;
                }
                IR[0]=memory[IC][0];
                IR[1]=memory[IC][1];
                IR[2]=memory[IC][2];
                IR[3]=memory[IC][3];
                IC++;

                if(IR[0]=="L"&&IR[1]=='R'){
                        String  line = new String(IR).substring(2);
                        int num = Integer.parseInt(line);
                        R[0]=memory[num][0];
                        R[1]=memory[num][1];
                        R[2]=memory[num][2];
                        R[3]=memory[num][3];
                }else if(IR[0]=='S'&&IR[1]=='R'){
                        int num = Integer.parseInt(newString(IR).substring(2));
                        memory[num][0]=R[0];
                        memory[num][1]=R[1]
                        memory[num][2]=R[2]
                        memory[num][3]=R[3]
                }else if(IR[0]=='C'&&IR[1]=='R'){
                        int num = Integer.parseInt(newString(IR).substring(2));
                        if(memory[num][0]==R[0]&&memory[num][1]==R[1]&&memory[num][2]==R[2]&&memory[num][3]==R[3]&&memory[num][4]){
                                T = 1;
                        }else{
                                T=0;
                        }
                }else if(IR[0]=='B'&&IR[1]=='T'){
                        if(T==1){
                                this.IC = Integer.parseInt(new String(IR).substring(2));
                                T=0;
                        }
                }else if(IR[0]=='G'&&IR[1]=='D'){
                        SI =1;
                        mos();
                }else if(IR[0]=='P'&&IR[1]=='D'){
                        SI =2;
                        mos();
                }else if(IR[0]=='H'){
                        SI =3;
                        mos();
                }
        }
        void mos(){
                if(SI==1){
                        read();
                }else if(SI==2){
                        write();
                }else if(SI==3){

                }
                SI=0;
        }
        void read(){
                int flag=0;
                IC[3]='0';
                String line = new String(IR);
                int num = Integer.parseInt(line.substring(3));
                try{
                        line = fread.readLine();
                }catch(Exception e){}
                buffer = line.toCharArrya();

                for(int i=0;i<line.length();){
                        memory[num][i%4]=buffer[i];
                        i++;
                        if(i%4==0){
                                num++;
                        }
                }
        }
        void write(){
                IR[3]='0';
                String line = newString(IR);
                int num = Integer.parseInt(line.substring(2));

                String t,total="";
                
                for(int i=0;i<line.length();){
                        t = new String(memory[num][i%4])
                        if(i%4==0){
                                num++;
                        }
                        total=total.concat(t);
                }
                try{
                        output.write(total);
                        output.write("\n");
                }
        }
}