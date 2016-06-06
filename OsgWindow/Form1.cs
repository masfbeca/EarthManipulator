using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ViewerExs;
using DevExpress.XtraSplashScreen;
using System.Windows;
using System.Threading;
namespace OsgWindow
{
    public partial class Form1 : Form
    {
        public ViewerEx _viewEx ;


        private bool _isInited = false;
        private bool _isPlaying = false;
        public Form1()
        {
            InitializeComponent();
            _viewEx =  new ViewerEx();
            timer1.Start();
            _viewEx.init(panel1.Handle, (uint)panel1.Width, (uint)panel1.Height);
            Thread.Sleep(1000);
        }

        private void Form1_Load(object sender, EventArgs e)
        {                               
            _viewEx.doFrame();
            radioButton1.Checked = true;
            SplashScreenManager.CloseForm();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            _viewEx.doFrame();
            if (_isPlaying)
            {          
                if (trackBar3.Value >= 48)
                {
                    trackBar3.Value = 0;                   
                }
                trackBar3.Value += 1;
                scrollDatetimeBar();
            }
        }

        private void Form1_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
           
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = "d:";//注意这里写路径时要用c:而不是c:　
           
            openFileDialog.Filter = "IVE模型(*.ive)|*.ive|OSG文件(*.osg)|*.osg|OSGT文件(*.osgt)|*.osgt|AutoCAD绘图交换文件|*.dxf|AutoCAD图形文件|*.dwg";
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 1;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                String fName = openFileDialog.FileName;
                
                _viewEx.setScene(fName);
                _isInited = true;
                initGlobe();
            }　
        }
        private void initGlobe()
        {
            if (_isInited)
            {
                string date = DateTime.Now.ToString("yyyy-MM-dd HH:mm");
                label6.Text = date;
                string[] hourAndM = date.Split(' ');
                string[] split = hourAndM[1].Split(':');
                int hours = int.Parse(split[0]);
                trackBar3.Value = hours * 2 + 1;
                _viewEx.setDateTime(trackBar3.Value / 2);    
            }
        }
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            _viewEx.setManipulator("EarthManipulator");
            groupBox2.Visible = true;
            groupBox3.Visible = false;
            groupBox3.Location = new System.Drawing.Point(3, 357);
            _isInited = false;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            _viewEx.setManipulator("FPSManipulator");
            groupBox3.Visible = true;
            groupBox2.Visible=false;
            groupBox3.Location = new System.Drawing.Point(3, 133);
        }

        private void 重置相机_Click(object sender, EventArgs e)
        {
            _viewEx.resetCamera();
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            panel1.Focus();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (_isInited)
            {

                try
                {
                    _viewEx.setViewpoint(float.Parse(txtLon.Text), float.Parse(txtLat.Text), float.Parse(txtHigh.Text));
                }
                catch (Exception exc)
                {
                    MessageBox.Show("输入格式有误");
                    txtLon.Focus();
                }
            }
        }

        private void toolStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            _viewEx.setSpeedFactor((float)(trackBar1.Value * 0.2));
        }

        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            _viewEx.setRotateSpeed((float)(trackBar2.Value * 0.2));
        }
        private void scrollDatetimeBar()
        {
            _viewEx.setDateTime(trackBar3.Value / 2);
            string datetime = ((float)trackBar3.Value / 2).ToString();
            string[] splitDatetime = datetime.Split('.');
            string hour = "";
            string minuts = "";
            if (splitDatetime.Length == 2)
            {
                hour = splitDatetime[0];
                minuts = "30";
            }
            else
            {
                hour = splitDatetime[0];
                minuts = "00";
            }
            label6.Text = DateTime.Now.ToString("yyyy-MM-dd") + " " + hour + ":" + minuts;
        }
        private void trackBar3_Scroll(object sender, EventArgs e)
        {
            if (_isInited)
            {
                scrollDatetimeBar();
            }
        }

        private void play_Click(object sender, EventArgs e)
        {
            if (_isInited)
            {
                if (_isPlaying)
                {
                    play.BackgroundImage = OsgWindow.Properties.Resources.play;
                    _isPlaying = false;
                }
                else
                {
                    play.BackgroundImage = OsgWindow.Properties.Resources.pause;
                    _isPlaying = true;
                }
            }
        }
    }
}
