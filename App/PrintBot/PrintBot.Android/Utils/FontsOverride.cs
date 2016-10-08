using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.Graphics;
using Java.Lang;

namespace PrintBot.Android.Utils
{
    public static class FontsOverride
    {
        public static void SetDefaultFont(Context context, string staticTypefaceFieldName, string fontAssetName)
        {
            var regular = Typeface.CreateFromAsset(context.Assets, "fonts/" + fontAssetName);
            ReplaceFont(staticTypefaceFieldName, regular);
        }

        private static void ReplaceFont(string staticTypefaceFieldName, Typeface newTypeface)
        {
            try
            {
                var staticField = Class.FromType(typeof(Typeface)).GetDeclaredField(staticTypefaceFieldName);
                staticField.Accessible = true;
                staticField.Set(null, newTypeface);
            }
            catch (NoSuchFieldException e)
            {
                e.PrintStackTrace();
            }
            catch (IllegalAccessException e)
            {
                e.PrintStackTrace();
            }
        }
    }
}