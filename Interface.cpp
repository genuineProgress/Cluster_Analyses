#include "Interface.h"

Interface::Interface (int id, bool write_log, Controller *c)
        : id (id), write_log (write_log), cc (c) {
    if (write_log) {
        log_file_name = "log.txt";
        logger = ofstream (log_file_name);
        if (!logger.is_open ()) {
            cout << "Error while writing logs" << endl;
            this->write_log = false;
        }
    }
}

Interface::~Interface () {
    if (logger.is_open ()) {
        logger.close ();
    }
}

int Interface::run () {
    // the function asks commands from console until gets the command exit
    cout << "Type HELP to see the list of supported commands." << endl;
    string cur_command; // command with parameters
    getline (cin, cur_command);
    while (manager (cur_command) != -1) {
        getline (cin, cur_command);
    }
    return 0;
}

void Interface::log (const string &s) {
    // writes s to log file
    if (write_log) {
        logger << s << endl;
    }
}

void Interface::show (const string &s) {
    // writes s to console and to log file
    cout << s << endl;
    log ("< " + s);
}

int Interface::run (ifstream &inp) {
    /* runs the program from filestream
     * -1 - file not found
     */
    if (!inp.is_open ()) {
        show ("File not found");
        return -1;
    }
    string cur_command_str;
    getline (inp, cur_command_str);
    while (manager (cur_command_str) != -1 && !inp.eof ()) {
        getline (inp, cur_command_str);
    }
    return 0;
}

int Interface::manager (const string &cur_command) {
    /* gets command for controller and sends it;
     * returns -1 for exit;
     */
    log ("> " + cur_command);
    stringstream ss (cur_command);
    string main;
    ss >> main;
    for (auto &c: main) c = toupper (c);
    if (main == "HELP") {
        // output of manual
        string manual;
        int err = cc->get_help (id, manual);
        if (err != 0) {
            show ("Mistake while getting data");
            log ("$ " + to_string (err));
        } else {
            ifstream help (manual);
            if (!help.is_open ()) {
                show ("Manual file not found");
                log ("$ " + manual);
            } else {
                string cur_line;
                log ("* file print:");
                while (getline (help, cur_line)) {
                    printf ("%s\n", cur_line.c_str ());
                    log ("< " + cur_line);
                }
                help.close ();
            }
        }
    } else if (main == "GC") {
        // generation of cloud(s)
        double x, y, disp_x, disp_y;
        int dots = 0;
        ss >> x >> y >> disp_x >> disp_y >> dots;
        int err=cc->generate_cloud (id, x, y, disp_x, disp_y, dots);
        if(err==-2) {
            show("Too many points in the cloud id.");
            log("Too many points in the cloud id.");
        }
        if (err==-3)
        {show("Not enough points in the cloud");
            log("Not enough points in the cloud");

        }
        else
            show ("Cloud created");

    } else if (main == "LOG") {
        string b;
        ss >> b;
        if (b == "1") {
            write_log = true;
            show ("Writing logs started");
        } else if (b == "0") {
            write_log = false;
            show ("Writing logs stopped");
        } else if (write_log) {
            show ("Logs are being written");
        } else {
            show ("Logs are not being written");
        }
    } else if (main == "PRINTF") {
        string file_name;
        ss >> file_name;
        int err = cc->fprintf (file_name);
        if (err == -1) {
            show ("Mistake while opening file");
            log ("$ " + file_name);
        }
    } else if (main == "HISTOGRAM") {
        int pieces;
        ss >> pieces;
        vector<int> x, y;
        cc->histogram (pieces, x, y);
        ofstream out ("histogram_x.txt");
        for (int i = 0; i < pieces; ++i) {
            out << x[i] << endl;
        }
        out.close ();
        out.open ("histogram_y.txt");
        for (int i = 0; i < pieces; ++i) {
            out << y[i] << endl;
        }
        out.close ();
        show ("Histogram saved to histogram_x.txt and histogram_y.txt");
    } else if (main == "EXIT") {
        show ("Finishing...");
        return -1;
    } else if (main == "BUFFER") {
        string operation;
        ss >> operation;
        for (auto &c: operation) c = toupper (c);
        if (operation == "UNLOAD") {
            if (cc->buffer_unload () == -1) {
                show ("Field in readonly mode");
            } else {
                show ("Buffer unloaded to the field");
            }
        } else if (operation == "ADD") {
            int cloud_id;
            ss >> cloud_id;
            if (cc->buffer_add_cloud (cloud_id) == -1) {
                show ("Field in readonly mode");
            } else {
                show ("Cloud №" + to_string (cloud_id) + " added to buffer");
            }
        } else if (operation == "ROTATE") {
            double angle;
            ss >> angle;
            if (cc->buffer_rotate (angle) == -1) {
                show ("Field in readonly mode");
            } else {
                show ("Buffer rotated");
            }
        } else if (operation == "ZOOM") {
            double k;
            ss >> k;
            if (cc->buffer_zoom (k) == -1) {
                show ("Field in readonly mode");
            } else {
                show ("Buffer zoomed");
            }
        } else if (operation == "SHIFT") {
            double x, y;
            ss >> x >> y;
            if (cc->buffer_shift (x, y) == -1) {
                show ("Field in readonly mode");
            } else {
                show ("Buffer shifted");
            }
        } else if (operation == "MIRROR") {
            if (cc->buffer_mirror () == -1) {
                show ("Field in readonly mode");
            } else {
                show ("Buffer reflected");
            }
        } else {
            // something went wrong
            show ("Unknown buffer operation. Check your input and try again.");
            show ("Type HELP to see the list of supported commands");
        }
    } else if (main == "MATRIX") {
        if (cc->matrix () == -1) {
            show ("Field not found");
        } else {
            show ("Field is readonly now. You can analyze it.");
            show ("Type HELP to see what you can do");
        }
    } else if (main == "WAVE") {
        int search_id;
        ss >> search_id;
        show("Clustering by means of wave algorithm");
        int err = cc->wave (search_id);
        if (err == 0) {
            show ("Field is clustered");
        } else if (err == -1) {
            show ("Check field state. Type MATRIX to set it to readonly");
        } else if (err == -2) {
            show ("Field doesn't exist. Type GC to create the clouds");
        } else if (err == -3) {
            show ("No search with such id. Type INFOCS to see accessible searches");
        }
    } else if (main == "DBSCAN") {
        int k;
        int search_id;
        ss >> search_id >> k;
        show ("Clustering...");
        int err = cc->dbscan (search_id, k);
        if (err == 0) {
            show ("Field is clustered");
        } else if (err == -1) {
            show ("Check field state. Type MATRIX to set it to readonly");
        } else if (err == -2) {
            show ("Field doesn't exist. Type GC to create the clouds");
        } else if (err == -3) {
            show ("No search with such id. Type INFOCS to see accessible searches");
        }
    } else if (main == "INFOCLUSTERSEARCH" || main == "INFOCS") {
        vector<Cluster_Search> searches = cc->info_cluster_search ();
        for (int i = 0; i < searches.size (); ++i) {
            show (to_string (i) + ": d = " + to_string (searches[i].delta) + "; clusters: "
                  + to_string (searches[i].clusters.size ()));
        }
    } else if (main == "BINARY") {
        //creating matrix of incidences
        double delta;
        ss >> delta;
        int err = cc->incidence_matrix(delta);
        if (err == 0) {
            show ("Incidence matrix is created. Now you can run clustering");
        } else if (err == -2) {
            show ("Field not found");
        }
        else if (err==-3)
        {
            show("Field is not in readonly state.");
            log("Field is not in readonly state.");
        }
    } else if (main == "STREE") {
        // searching minimal spanning tree
        vector<int> histogram = cc->s_tree ();
        for (int i : histogram) {
            show (to_string (i));
        }
    } else if (main == "KMEANS") {
        int k;
        ss >> k;
        show ("Clustering...");
        int err = cc->k_means (k);
        if (err == 0) {
            show ("Field is clustered");
        } else if (err == -1) {
            show ("Check field state. Type MATRIX to set it to readonly");
        } else if (err == -2) {
            show ("Field doesn't exist. Type GC to create the clouds");
        }
    } else if (main == "KMCORES") {
        // K-means with cores
        int clusters_number = 0;
        int cores_number = 0;
        ss >> clusters_number >> cores_number;
        show ("Clustering...");
        int err = cc->k_means_cores (clusters_number, cores_number);
        if (err == 0) {
            show ("Field is clustered");
        } else if (err == -1) {
            show ("Check field state. Type MATRIX to set it to readonly");
        } else if (err == -2) {
            show ("Field doesn't exist. Type GC to create the clouds");
        }
    } else if (main == "EM") {
        int k;
        ss >> k;
        show ("Clustering...");
        int err = cc->em (k);
        if (err == 0) {
            show ("Field is clustered");
        } else if (err == -1) {
            show ("Check field state. Type MATRIX to set it to readonly");
        } else if (err == -2) {
            show ("Field doesn't exist. Type GC to create the clouds");
        }
    } else if (main == "HA") {
        show ("Clustering...");
        int err = cc->hierarchical_algorithm ();
        if (err == 0) {
            show ("Field is clustered");
        } else if (err == -1) {
            show ("Check field state. Type MATRIX to set it to readonly");
        } else if (err == -2) {
            show ("Field doesn't exist. Type GC to create the clouds");
        }
    } else if (main == "FOREL") {
        show ("Clustering...");
        int err = cc->forel ();
        if (err == 0) {
            show ("Field is clustered");
        } else if (err == -1) {
            show ("Check field state. Type MATRIX to set it to readonly");
        } else if (err == -2) {
            show ("Field doesn't exist. Type GC to create the clouds");
        }
    } else if (main == "DELAUNAY") {
        int err = cc->delaunay ();
        if (err == 0) {
            show ("Field is triangulated");
        } else if (err == -1) {
            show ("Check field state. Type MATRIX to set it to readonly");
        } else if (err == -2) {
            show ("Field doesn't exist. Type GC to create the clouds");
        } else if (err == -3) {
            show ("Not enough points for triangulation");
        }
    } else if (main[0] == '#') {
        // doing nothing, that's comment
    }
    else if(main== "PLOT")
    {
        int err=cc->plot();
        if(err==0) {
            show ("Plot file is created");
        }
        else if (err==1) {
            show("No cloud is created. Try to generate some clouds before.");
        }
        else if (err==2)
            show("Some kind of error occured. Fixing is in progress...");
    }
    else if(main=="");
    else
    {
        // something went wrong
        show ("Unknown command. Check your input and try again.");
        show ("Type HELP to see the list of supported commands");
    }
    return 0;
}