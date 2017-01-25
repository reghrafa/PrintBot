using Android.App;
using Android.Widget;
using Android.OS;
using System;
using System.Collections.Generic;
using static Android.Views.ViewGroup;
using PrintBot.Infrastructure.ViewModels;
using PrintBot.Domain.Models;
using System.IO;
using System.Threading.Tasks;
using Android;
using PrintBot.Droid.Utils;
using Android.Graphics.Drawables;
using Android.Graphics;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "Bord Editor", WindowSoftInputMode = Android.Views.SoftInput.AdjustNothing, MainLauncher = false, Icon = "@drawable/icon")]
    public class BordEditor_MainActivity : PortraitActivity
    {

        public static float _scaleFactor;
        public static float _screenWidth;

        private BordEditor_BordPin _boradPinTmp = null;
        private BordEditor_Modul.ModulButton _modulTmp = null;
        private RelativeLayout _mainLayout;
        private BordEditor_Bord bord;
        private List<string> _modulFileNames = new List<string>();
        private ModuleSetupViewModel _vm;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            SetContentView(Resource.Layout.BordEditor_MainLayout);

            ColorDrawable colorDrawable = new ColorDrawable(Color.ParseColor("#3b8686"));
            ActionBar.SetBackgroundDrawable(colorDrawable);

            _vm = ServiceLocator.Current.ModuleSetupViewModel;
            GetModulFileNames();//get ModulFiles

            _mainLayout = FindViewById<RelativeLayout>(Resource.Id.BordEditor_MainLayout);

            _scaleFactor = Resources.DisplayMetrics.Density;
            _screenWidth = Resources.DisplayMetrics.WidthPixels;

            // Create MSlots at _mSlotPositions          
            for (int i = 0; i < 2; i++)
            {

                // if has no reason it is just so, id needed a Selector
                bool tmpBool = i % 2 == 0 ? true : false;

                CreateModulSlot(tmpBool);

              /*  var tmp = new BordEditor_ModulSlot(this, new BordEditor_ListAdapter(this, _modulFileNames),tmpBool);
                _mainLayout.AddView(tmp);

                tmp.loadNamesBtn.Click += delegate
                {
                    GetModulFileNames();
                    tmp.Adapter.NotifyDataSetChanged();
                };
                tmp.CreateBtn.Click += delegate
                {
                    ReplaceModulSlot(tmp);
                };

                tmp.ModuleFileNamesList.ItemClick +=
                    (object o, AdapterView.ItemClickEventArgs e) => ModulList_ItemClick(o, e, tmp);
                */
                
              
            }
            //Bord
            BordInit();

        }

        private async void ModulList_ItemClick(object sender, AdapterView.ItemClickEventArgs e, BordEditor_ModulSlot m)
        {
            await ReplaceModulSlot(m, _modulFileNames[e.Position]);
        }

        public async void GetModulFileNames()
        {
            List<BordEditor_ModulPhysical> tmp = new List<BordEditor_ModulPhysical>();
            _modulFileNames.Clear();
            tmp = await _vm.LoadModuleAsync();//get Module
            foreach (var item in tmp)
            {
                _modulFileNames.Add(item.Name); //Get Module FileNames
            }

        }

        private BordEditor_ModulSlot CreateModulSlot(bool isRight)
        {

            var tmp = new BordEditor_ModulSlot(this, new BordEditor_ListAdapter(this, _modulFileNames), isRight);
            _mainLayout.AddView(tmp);

            tmp.loadNamesBtn.Click += delegate
            {
                GetModulFileNames();
                tmp.Adapter.NotifyDataSetChanged();
            };
            tmp.CreateBtn.Click += delegate
            {
                ReplaceModulSlot(tmp);
            };

            tmp.ModuleFileNamesList.ItemClick +=
                (object o, AdapterView.ItemClickEventArgs e) => ModulList_ItemClick(o, e, tmp);

            return tmp;
        }

        private void BordInit()
        {
            //Each Pin needs a Paintview to Draw a Conection
            bord = new BordEditor_Bord(this);
            _mainLayout.AddView(bord);
            

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
            //Add Pinn function
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
                    _modulTmp.ConnectePin.pw.Clear(); // clear old connected Wire
                    _modulTmp.ConnectePin = null;
                }
                catch { }

                try
                {
                    _boradPinTmp.ConectetPin.reset(); // reset old connected ModulPin
                    _boradPinTmp.ConectetPin.ConnectePin = null;
                    _boradPinTmp.ConectetPin = null;
                }
                catch { }

                _boradPinTmp.pw.Clear(); // clear old Wire
                
                //setReferenzes
                _modulTmp.ConnectePin = _boradPinTmp; 
                _boradPinTmp.ConectetPin = _modulTmp;

                // Grafik
                _boradPinTmp.pw.connectPins(_boradPinTmp, _modulTmp); //Draw new Line
                // let the ModulPin Look like the conected Board Pin
                _modulTmp.Text = _modulTmp.ConnectePin.Text;
                _modulTmp.SetTextColor(_modulTmp.ConnectePin.TextColors);
                _modulTmp.SetBackgroundColor(_modulTmp.ConnectePin.BackgroundColor);

                // reset
                _boradPinTmp = null;
                _modulTmp = null;
            }
        }

        void ReplaceModulSlot(BordEditor_ModulSlot slot)
        {
            var modul = new BordEditor_Modul(this, slot.PinCount);
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

        async Task ReplaceModulSlot(BordEditor_ModulSlot slot, string filename)
        {

            var loadedModul = await _vm.LoadModuleAsync();
            int modulIndex = GetModulFileIndexByName(filename, loadedModul);//get Index of wanted Modul
            BordEditor_Modul modul = new BordEditor_Modul(this, loadedModul[modulIndex]);
            modul.SetModulTyp(loadedModul[modulIndex].ModulType);
            _mainLayout.AddView(modul);
            modul.selfDestrucktion.Click += delegate
            {
                foreach (BordEditor_Modul.ModulButton pin in modul.modulPins)
                {
                    try
                    {
                        pin.ConnectePin.pw.Clear();
                    }
                    catch { }
                    var newModulSlot = CreateModulSlot(true);
                    newModulSlot.TranslationX = modul.GetX();

                }
                _mainLayout.RemoveView(modul);
            };
            modul.TranslationX = slot.GetX();
            modul.TranslationY = slot.GetY();
            modul.SwitchPinSideIfNeeded();
           // _mainLayout.RemoveView(slot);
            

            int i = 0;
            foreach (BordEditor_Modul.ModulButton modulPin in modul.modulPins)
            {
                modulPin.Click += delegate { setModulPin(modulPin); };

                BordEditor_PysicalPin index = loadedModul[modulIndex].PinList[i];

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

                if (index.PinType != null)
                {
                    // if Pin ist not set do no conection
                    // modulPin.ConnectePin.pw.connectPins(modulPin.ConnectePin, modulPin);
                    setModulPin(modulPin);
                    setPlatinePin(modulPin.ConnectePin);
                    pinConect(); 
                }

                i++;
            }

        }

        private int GetModulFileIndexByName(String name, List<BordEditor_ModulPhysical> modules)
        {
            int i = 0;
            foreach (var item in modules)
            {
                if (item.Name == name)
                {
                    return i;
                }
                i++;
            }
            return 0;

        }

    }
}