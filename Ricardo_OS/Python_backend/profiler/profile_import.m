%% Import data from text file
% Script for importing data from the following text file:
%
%    filename: /Users/kiran/Documents/Projects/ICLR/Avionics/Ricardo_OS/Python_backend/profile.txt
%
% Auto-generated by MATLAB on 12-Sep-2021 00:48:18

%% Set up the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 2);

% Specify range and delimiter
opts.DataLines = [2, Inf];
opts.Delimiter = ":";

% Specify column names and types
opts.VariableNames = ["cat", "time"];
opts.VariableTypes = ["categorical", "double"];

% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";

% Specify variable properties
opts = setvaropts(opts, "cat", "EmptyFieldRule", "auto");

% Import the data
profile1 = readtable("/Users/kiran/Documents/Projects/ICLR/Avionics/Ricardo_OS/Python_backend/profile.txt", opts);


%% Clear temporary variables
clear opts

profile_table.log = table2array(profile1(profile1.cat=='log',2))
profile_table.logc = table2array(profile1(profile1.cat=='logc',2))
profile_table.est = table2array(profile1(profile1.cat=='est',2))
profile_table.net = table2array(profile1(profile1.cat=='net',2))
profile_table.sens = table2array(profile1(profile1.cat=='sens',2))
profile_table.tunez = table2array(profile1(profile1.cat=='tunez',2))

figure
hold on

plot(profile_table.log)
plot(profile_table.logc)
plot(profile_table.est)
plot(profile_table.net)
plot(profile_table.sens)
plot(profile_table.tunez)

legend('log','logc','est','net','sens','tunez')