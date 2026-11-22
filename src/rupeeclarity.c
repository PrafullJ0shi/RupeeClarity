/*
 * RupeeClarity: Intelligent Interest Simulation and Wealth Explanation System
 * Mini Project for BCA-I, Programming in C Lab (BCA-C112)
 * Uttaranchal School of Computing Sciences, Uttaranchal University, Dehradun
 *
 * Submitted by: Prafull Joshi
 * Class Roll No.: 43
 * Submitted to: Ms. Shivani Sisodia
 * Session: 2026-27
 *
 * Compile: gcc rupeeclarity.c -o rupeeclarity -lm
 * Run:     ./rupeeclarity
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

/* Function prototypes */
void clear_screen(void);
void pause_screen(void);
void clean_input(void);
int read_choice(int min, int max);
double read_positive(const char *prompt);
void ask_prt(double *P, double *R, double *T);
int ask_n(void);
void print_header(const char *title);

void module_si_lab(void);
void module_ci_lab(void);
void module_compare(void);
void module_year_table(void);
void module_goal_check(void);
void module_what_if(void);
void module_double(void);
void module_story(void);
void module_report(void);
void module_formulas(void);
void module_about(void);

/* Helper functions */
void clear_screen(void) {
    system(CLEAR);
}

void pause_screen(void) {
    printf("\nPress ENTER to return to menu...");
    clean_input();
    getchar();
}

void clean_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int read_choice(int min, int max) {
    int choice;
    char buffer[100];
    while (1) {
        printf("Enter choice (%d-%d): ", min, max);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        if (sscanf(buffer, "%d", &choice) == 1 && choice >= min && choice <= max) {
            return choice;
        }
        printf("Invalid choice. Please try again.\n");
    }
}

double read_positive(const char *prompt) {
    double val;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        if (sscanf(buffer, "%lf", &val) == 1 && val > 0) {
            return val;
        }
        printf("Please enter a positive number.\n");
    }
}

void ask_prt(double *P, double *R, double *T) {
    *P = read_positive("Enter Principal (P) in Rs: ");
    *R = read_positive("Enter Rate of Interest (R) in %: ");
    *T = read_positive("Enter Time (T) in years: ");
}

int ask_n(void) {
    printf("\nCompounding frequency:\n");
    printf("1. Yearly     (n = 1)\n");
    printf("2. Half-yearly (n = 2)\n");
    printf("3. Quarterly  (n = 4)\n");
    printf("4. Monthly    (n = 12)\n");
    int choice = read_choice(1, 4);
    switch (choice) {
        case 1: return 1;
        case 2: return 2;
        case 3: return 4;
        case 4: return 12;
        default: return 1;
    }
}

void print_header(const char *title) {
    clear_screen();
    printf("============================================================\n");
    printf("           RUPEECLARITY - %s\n", title);
    printf("============================================================\n\n");
}

/* Module 1: Simple Interest Lab */
void module_si_lab(void) {
    double P, R, T, SI, Amount;
    print_header("SIMPLE INTEREST LAB");
    ask_prt(&P, &R, &T);
    SI = (P * R * T) / 100.0;
    Amount = P + SI;

    printf("\n--- Working ---\n");
    printf("SI = (P x R x T) / 100\n");
    printf("SI = (%.2f x %.2f x %.2f) / 100 = %.2f\n", P, R, T, SI);
    printf("Amount = P + SI = %.2f + %.2f = %.2f\n", P, SI, Amount);
    printf("\nResult:\n");
    printf("Simple Interest : Rs %.2f\n", SI);
    printf("Total Amount    : Rs %.2f\n", Amount);
    pause_screen();
}

/* Module 2: Compound Interest Lab */
void module_ci_lab(void) {
    double P, R, T, A, CI;
    int n;
    print_header("COMPOUND INTEREST LAB");
    ask_prt(&P, &R, &T);
    n = ask_n();
    A = P * pow(1 + R / (100.0 * n), n * T);
    CI = A - P;

    printf("\n--- Working ---\n");
    printf("A = P x (1 + R/(100*n))^(n*T)\n");
    printf("n = %d\n", n);
    printf("A = %.2f x (1 + %.2f/(100*%d))^(%.0f) = %.2f\n", P, R, n, n * T, A);
    printf("CI = A - P = %.2f - %.2f = %.2f\n", A, P, CI);
    printf("\nResult:\n");
    printf("Compound Interest : Rs %.2f\n", CI);
    printf("Total Amount      : Rs %.2f\n", A);
    pause_screen();
}

/* Module 3: Compare SI and CI */
void module_compare(void) {
    double P, R, T, SI, SI_Amt, A, CI, extra;
    int n;
    print_header("COMPARE SI AND CI");
    ask_prt(&P, &R, &T);
    n = ask_n();

    SI = (P * R * T) / 100.0;
    SI_Amt = P + SI;
    A = P * pow(1 + R / (100.0 * n), n * T);
    CI = A - P;
    extra = CI - SI;

    printf("\n--- Comparison ---\n");
    printf("Simple Interest     : Rs %.2f\n", SI);
    printf("SI Amount           : Rs %.2f\n", SI_Amt);
    printf("Compound Interest   : Rs %.2f\n", CI);
    printf("CI Amount           : Rs %.2f\n", A);
    printf("Extra (CI - SI)     : Rs %.2f\n", extra);
    if (extra > 0)
        printf("Winner              : Compound Interest\n");
    else
        printf("Winner              : Simple Interest (or equal)\n");
    printf("\nReason: Last year's interest starts earning interest in compounding.\n");
    pause_screen();
}

/* Module 4: Year Table */
void module_year_table(void) {
    double P, R, T;
    int n, years, y;
    print_header("YEAR-BY-YEAR TABLE");
    ask_prt(&P, &R, &T);
    n = ask_n();
    years = (int)T;
    if (years < 1) years = 1;
    if (years > 30) years = 30;  /* safety limit */

    printf("\nYear | SI Amount     | CI Amount     | Gap\n");
    printf("-----|---------------|---------------|--------\n");
    for (y = 1; y <= years; y++) {
        double si_amt = P + (P * R * y) / 100.0;
        double ci_amt = P * pow(1 + R / (100.0 * n), n * y);
        double gap = ci_amt - si_amt;
        printf("%4d | Rs %10.2f | Rs %10.2f | Rs %.2f\n", y, si_amt, ci_amt, gap);
    }
    pause_screen();
}

/* Module 5: Goal Check */
void module_goal_check(void) {
    double P, R, T, target, SI_Amt, CI_Amt;
    int n;
    print_header("GOAL CHECK");
    ask_prt(&P, &R, &T);
    n = ask_n();
    target = read_positive("Enter Target Amount in Rs: ");

    SI_Amt = P + (P * R * T) / 100.0;
    CI_Amt = P * pow(1 + R / (100.0 * n), n * T);

    printf("\nTarget: Rs %.2f\n", target);
    printf("SI Amount: Rs %.2f  --> %s\n", SI_Amt, (SI_Amt >= target) ? "REACHED" : "NOT REACHED");
    printf("CI Amount: Rs %.2f  --> %s\n", CI_Amt, (CI_Amt >= target) ? "REACHED" : "NOT REACHED");
    pause_screen();
}

/* Module 6: What-If Rate +1% */
void module_what_if(void) {
    double P, R, T, A1, A2;
    int n;
    print_header("WHAT-IF: RATE +1%");
    ask_prt(&P, &R, &T);
    n = ask_n();

    A1 = P * pow(1 + R / (100.0 * n), n * T);
    A2 = P * pow(1 + (R + 1) / (100.0 * n), n * T);

    printf("\nCI Amount at %.2f%% : Rs %.2f\n", R, A1);
    printf("CI Amount at %.2f%% : Rs %.2f\n", R + 1, A2);
    printf("Difference          : Rs %.2f\n", A2 - A1);
    pause_screen();
}

/* Module 7: Years to Double (Rule of 72) */
void module_double(void) {
    double R, years;
    print_header("YEARS TO DOUBLE (RULE OF 72)");
    R = read_positive("Enter Rate of Interest (R) in %: ");
    years = 72.0 / R;
    printf("\nApproximate years to double the money: %.2f years\n", years);
    printf("(Rule of 72 is a shortcut, not exact.)\n");
    pause_screen();
}

/* Module 8: Story Mode */
void module_story(void) {
    double P, R, T, SI, CI, extra;
    int n;
    print_header("STORY MODE");
    ask_prt(&P, &R, &T);
    n = ask_n();

    SI = (P * R * T) / 100.0;
    CI = P * pow(1 + R / (100.0 * n), n * T) - P;
    extra = CI - SI;

    printf("\nSimple Interest is like a fixed salary.\n");
    printf("Every year you get the same extra money (Rs %.2f), and it never goes back to work.\n", SI / T);
    printf("\nCompound Interest is like a salary that works again.\n");
    printf("Last year's interest is put back into the principal, so it starts earning more.\n");
    printf("\nThat is why Compound Interest gives you Rs %.2f extra.\n", extra);
    printf("The extra is not a rounding error. It is last year's interest going back to work.\n");
    pause_screen();
}

/* Module 9: Save Report File */
void module_report(void) {
    double P, R, T, SI, SI_Amt, A, CI, extra;
    int n;
    FILE *fp;
    print_header("SAVE REPORT FILE");
    ask_prt(&P, &R, &T);
    n = ask_n();

    SI = (P * R * T) / 100.0;
    SI_Amt = P + SI;
    A = P * pow(1 + R / (100.0 * n), n * T);
    CI = A - P;
    extra = CI - SI;

    fp = fopen("rupeeclarity_report.txt", "w");
    if (fp == NULL) {
        printf("Error: Could not create report file.\n");
    } else {
        fprintf(fp, "RUPEECLARITY REPORT\n");
        fprintf(fp, "===================\n\n");
        fprintf(fp, "Principal (P) : Rs %.2f\n", P);
        fprintf(fp, "Rate (R)      : %.2f%%\n", R);
        fprintf(fp, "Time (T)      : %.2f years\n", T);
        fprintf(fp, "Compounding n : %d\n\n", n);
        fprintf(fp, "Simple Interest   : Rs %.2f\n", SI);
        fprintf(fp, "SI Amount         : Rs %.2f\n", SI_Amt);
        fprintf(fp, "Compound Interest : Rs %.2f\n", CI);
        fprintf(fp, "CI Amount         : Rs %.2f\n", A);
        fprintf(fp, "Extra (CI - SI)   : Rs %.2f\n", extra);
        fprintf(fp, "\nGenerated by RupeeClarity - USCS, Uttaranchal University\n");
        fclose(fp);
        printf("Report saved successfully as 'rupeeclarity_report.txt'\n");
        printf("You can open it in Notepad or any text editor.\n");
    }
    pause_screen();
}

/* Module 10: Formula Vault */
void module_formulas(void) {
    print_header("FORMULA VAULT");
    printf("SIMPLE INTEREST\n");
    printf("SI = (P x R x T) / 100\n");
    printf("Amount = P + SI\n\n");
    printf("COMPOUND INTEREST\n");
    printf("A = P x (1 + R/(100*n))^(n*T)\n");
    printf("CI = A - P\n\n");
    printf("RULE OF 72\n");
    printf("Years to double ≈ 72 / R\n\n");
    printf("Where:\n");
    printf("P = Principal, R = Rate (%%), T = Time (years), n = compounding frequency\n");
    pause_screen();
}

/* Module 11: About / Project Card */
void module_about(void) {
    print_header("ABOUT RUPEECLARITY");
    printf("Project : RupeeClarity - Intelligent Interest Simulation\n");
    printf("          and Wealth Explanation System\n\n");
    printf("Submitted by : Prafull Joshi\n");
    printf("Class Roll No: 43\n");
    printf("Programme    : BCA - I Year\n");
    printf("Session      : 2026-27\n\n");
    printf("Submitted to : Ms. Shivani Sisodia\n");
    printf("Course       : Programming in C Lab (BCA-C112)\n");
    printf("Institute    : Uttaranchal School of Computing Sciences\n");
    printf("               Uttaranchal University, Dehradun\n\n");
    printf("A menu-driven C program that calculates, compares and explains\n");
    printf("Simple Interest and Compound Interest without hiding the working.\n");
    pause_screen();
}

/* Main menu */
int main(void) {
    int choice;
    while (1) {
        clear_screen();
        printf("============================================================\n");
        printf("                  R U P E E C L A R I T Y\n");
        printf("     Intelligent Interest Simulation & Wealth Explanation\n");
        printf("============================================================\n");
        printf("USCS  ·  BCA-I  ·  2026-27\n\n");
        printf("  1. Simple Interest Lab\n");
        printf("  2. Compound Interest Lab\n");
        printf("  3. Compare SI and CI\n");
        printf("  4. Year-by-Year Table\n");
        printf("  5. Goal Check\n");
        printf("  6. What-If Rate +1%%\n");
        printf("  7. Years to Double (Rule of 72)\n");
        printf("  8. Story Mode\n");
        printf("  9. Save Report File\n");
        printf(" 10. Formula Vault\n");
        printf(" 11. About / Project Card\n");
        printf("  0. Exit\n");
        printf("\n");

        choice = read_choice(0, 11);

        switch (choice) {
            case 1: module_si_lab(); break;
            case 2: module_ci_lab(); break;
            case 3: module_compare(); break;
            case 4: module_year_table(); break;
            case 5: module_goal_check(); break;
            case 6: module_what_if(); break;
            case 7: module_double(); break;
            case 8: module_story(); break;
            case 9: module_report(); break;
            case 10: module_formulas(); break;
            case 11: module_about(); break;
            case 0:
                printf("\nThank you for using RupeeClarity.\n");
                printf("The extra hundred is last year's interest, going back to work.\n");
                return 0;
        }
    }
    return 0;
}
