using Android.App;
using Android.Widget;
using Android.OS;
using System;
using System.Collections.Generic;
using static Android.Views.ViewGroup;
using System.IO;


namespace PrintBot.Android
{
    [Activity(Label = "XamPaint", MainLauncher = false, Icon = "@drawable/icon")]
    public class BordEditor_MainActivity : Activity
    {
        private BordEditor_BordPin _boradPinTmp = null;
        private BordEditor_Modul.ModulButton _modulTmp = null;
        private RelativeLayout _mainLayout;
        private BordEditor_Bord bord;
        private List<string> _modulFileNames;

        private LinearLayout modulSlot1;
        private LinearLayout modulSlot2;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            SetContentView(Resource.Layout.BordEditor_MainLayout);
            _mainLayout = FindViewById<RelativeLayout>(Resource.Id.BordEditor_MainLayout);

            //Adapter
            _modulFileNames = GetModulFiles();
            BordEditor_ListAdapter adapter = new BordEditor_ListAdapter(this, _modulFileNames);

            //ModulSlot1
            modulSlot1 = FindViewById<LinearLayout>(Resource.Id.BordEditor_modulSlot1);
            Button modulSlot1Button = FindViewById<Button>(Resource.Id.BordEditor_modulSlot1Button);
            modulSlot1Button.Click += delegate { ReplaceModulSlot(modulSlot1, Resource.Id.BordEditor_modulSlot1Text); };            
            //livtView 1
            var modullist1 = FindViewById<ListView>(Resource.Id.BordEditor_modulList1);
            modullist1.Adapter = adapter;
            modullist1.ItemClick += Modullist1_ItemClick;

            //ModulSlot2
            modulSlot2 = FindViewById<LinearLayout>(Resource.Id.BordEditor_modulSlot2);
            Button modulSlotButton2 = FindViewById<Button>(Resource.Id.BordEditor_modulSlotButton2);
            modulSlot1Button.Click += delegate { ReplaceModulSlot(modulSlot2, Resource.Id.BordEditor_modulSlot2); };
            //livtView 2
            var modullist2 = FindViewById<ListView>(Resource.Id.BordEditor_modulList2);
            modullist2.Adapter = adapter;
            modullist2.ItemClick += Modullist2_ItemClick;  

            //Bord
            BordInit();

        }

        private void Modullist2_ItemClick(object sender, AdapterView.ItemClickEventArgs e)
        {
            ReplaceModulSlot(modulSlot2, _modulFileNames[e.Position]);
        }

        private void Modullist1_ItemClick(object sender, AdapterView.ItemClickEventArgs e)
        {
            ReplaceModulSlot(modulSlot1, _modulFileNames[e.Position]);
        }

        private List<string> GetModulFiles()
        {
            string path = System.Environment.GetFolderPath(System.Environment.SpecialFolder.Personal);
            var dir = new DirectoryInfo(path);
            List<string> module = new List<string>();

            foreach (var item in dir.GetFileSystemInfos())
            {
                if (item.ToString().Contains("Modul_"))
                {
                    string file = item.ToString();
                    file = file.Substring(file.IndexOf("Modul_"));
                    file = file.Substring(file.IndexOf("Modul_"), file.IndexOf("."));
                    module.Add(file);
                }
            }

            
            return module;
        }

        private void BordInit()
        {
            bord = FindViewById<BordEditor_Bord>(Resource.Id.BordEditor_Bord);
            foreach (BordEditor_BordPin p in bord.DigitalPins)
            {
                AddPaintViewToPin(p);
            }
            foreach (BordEditor_BordPin p in bord.AnalogPins)
            {
                AddPaintViewToPin(p);
            }
            AddPaintViewToPin(bord.Pin3V);
            AddPaintViewToPin(bord.Pin5V);
            AddPaintViewToPin(bord.PinVin);

        }

        private void AddPaintViewToPin(BordEditor_BordPin p)
        {
            p.Click += delegate { setPlatinePin(p); };
            var paint = new BordEditor_PaintView(this);
            paint.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, LayoutParams.MatchParent);
            paint._drawPaint = p.pw._drawPaint;
            _mainLayout.AddView(paint);
            p.pw = paint;
        }

        void setPlatinePin(BordEditor_BordPin p)
        {
            _boradPinTmp = p;
            pinConect();
        }

        void setModulPin(BordEditor_Modul.ModulButton b)
        {
            _modulTmp = b;
            pinConect();
        }

        void pinConect()
        {
            if (_boradPinTmp != null && _modulTmp != null)
            {
                try
                {
                    _modulTmp.ConnectePin.pw.Clear(); // clear connected Wire
                }
                catch { }

                _boradPinTmp.pw.Clear(); // clear aktual Wire
                _modulTmp.ConnectePin = null; // clear connected pen

                _modulTmp.ConnectePin = _boradPinTmp; // set connected pin
                _boradPinTmp.pw.connectPins(_boradPinTmp, _modulTmp); // Grafik
                _modulTmp.Modul.AttachSimulator(_boradPinTmp.Nr); // Real 

                _boradPinTmp = null;
                _modulTmp = null;
            }
        }

        void ReplaceModulSlot(LinearLayout slot, int TextId)
        {
            int count = 2;
            try
            {
                var eText = FindViewById<EditText>(TextId).Text;
                count = Int32.Parse(eText);

                if (count < 2)
                {
                    count = 2;
                }

            }
            catch
            {
                count = 2;
            }


            var modul = new BordEditor_Modul(this, count);
            _mainLayout.AddView(modul);

            foreach (BordEditor_Modul.ModulButton pin in modul.modulPins)
            {
                pin.Click += delegate { setModulPin(pin); };
            }

            modul.selfDestrucktion.Click += delegate
            {
                foreach (BordEditor_Modul.ModulButton pin in modul.modulPins)
                {
                    try
                    {
                        pin.ConnectePin.pw.Clear();
                    }
                    catch (NullReferenceException e) { }

                }
                _mainLayout.RemoveView(modul);
            };
            modul.TranslationX = slot.GetX();
            modul.TranslationY = slot.GetY();
            modul.SwitchPinSideIfNeeded();

        }

        void ReplaceModulSlot(LinearLayout slot, string filename)
        {
            var loadedModul = BordEditor_ModulPhysical.LoadModul(filename);
            BordEditor_Modul modul = new BordEditor_Modul(this, loadedModul);
            _mainLayout.AddView(modul);
            modul.selfDestrucktion.Click += delegate
            {
                foreach (BordEditor_Modul.ModulButton pin in modul.modulPins)
                {
                    try{
                        pin.ConnectePin.pw.Clear();
                    }catch { }

                }
                _mainLayout.RemoveView(modul);
            };
            modul.TranslationX = slot.GetX();
            modul.TranslationY = slot.GetY();
            modul.SwitchPinSideIfNeeded();

            int i = 0;
            foreach (BordEditor_Modul.ModulButton modulPin in modul.modulPins)
            {
                modulPin.Click += delegate { setModulPin(modulPin); };

                BordEditor_PysicalPin index = loadedModul.PinList[i];

                if (index.PinType == "Digital")
                {
                    modulPin.ConnectePin = bord.DigitalPins[index.PinNr];
                }
                else if (index.PinType == "Analog")
                {
                    modulPin.ConnectePin = bord.AnalogPins[index.PinNr];
                }
                else if (index.PinType == "V3")
                {
                    modulPin.ConnectePin = bord.Pin3V;
                }
                else if (index.PinType == "V5")
                {
                    modulPin.ConnectePin = bord.Pin5V;
                }
                else if (index.PinType == "Vin")
                {
                    modulPin.ConnectePin = bord.PinVin;
                }

                modulPin.ConnectePin.pw.connectPins(modulPin.ConnectePin, modulPin);
                i++;
            }

        }
    }
}