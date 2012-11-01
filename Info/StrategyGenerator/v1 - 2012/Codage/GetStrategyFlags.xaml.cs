﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using StrategyGenerator.Strategy;

namespace StrategyGenerator
{
    /// <summary>
    /// Logique d'interaction pour Window1.xaml
    /// </summary>
    public partial class GetStrategyFlags : Window
    {
        public GetStrategyFlags(String CurrentFlags)
        {
            InitializeComponent();

            foreach (EnumStrategyFlag EFlags in Enum.GetValues(typeof(EnumStrategyFlag)))
            {
                _ValueFlags.Add(EFlags.ToString());
            }

            for (int i = 0; i < _ValueFlags.Count; i++)
            {
                listBox_Flags.Items.Add(_ValueFlags[i].ToString());
            }


            // Parse current flags
            CurrentFlags = CurrentFlags.Replace("(", "");
            CurrentFlags = CurrentFlags.Replace(")", "");
            CurrentFlags = CurrentFlags.Replace(" ", "");

            string[] ParsedString = CurrentFlags.Split(new char[]{'+'});
            for (int i = 0; i < ParsedString.Length; i++)
            {
                listBox_Flags.SelectedItems.Add(ParsedString[i]);
            }
        }

        private void Validate_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void listBox_Flags_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            _SelectedFlags = "(";

            for (int i = 0; i < listBox_Flags.SelectedItems.Count; i++)
            {
                _SelectedFlags += listBox_Flags.SelectedItems[i].ToString();

                if (i < listBox_Flags.SelectedItems.Count - 1)
                    _SelectedFlags += " + ";
            }

            _SelectedFlags += ")";

            textBoxValue.Text = _SelectedFlags;
        }

        public String SelectedFlags { get { return _SelectedFlags; } }

        private String _SelectedFlags = "";
        private List<String> _ValueFlags = new List<string>();

    }
}