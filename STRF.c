#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[100];
    int bt; //burst time (execution time)
    int wt; //waiting time
    int tt; //turnaround time
    int arrive; //arrival time
    int rank; //priority
    int cmplt; //complete time
    int stay; //stay time aka response time
} arr;

void swap ( arr *a, arr *b ) {
    arr temp = *a;
    *a = *b;
    *b = temp;
}

void ibtsort ( arr *x, int n, int ct ) {
    for ( arr *i = x; (i < x + n - 1) && ((i + 1) -> arrive <= ct); ++i )
        for ( arr *j = x; (j < x + n - 1) && ((j + 1) -> arrive <= ct); ++j )
            if ( j -> bt > (j + 1) -> bt )
                swap( j, j + 1);
}

void timesort ( arr *x, int n ) {
    for ( arr *i = x; i < x + n - 1; ++i )
        for ( arr *j = x; j < x + n - 1; ++j )
            if ( j -> arrive > (j + 1) -> arrive )
                swap( j, j + 1);
}

void ranksort ( arr *x, int n ) {
    for ( arr *i = x; i < x + n - 1; ++i )
        for ( arr *j = x; j < x + n - 1; ++j )
            if ( j -> rank > (j + 1) -> rank )
                swap( j, j + 1);
}

void read ( arr *x, int n ) {
    int i = 0;
    for ( arr *p = x; p < x + n; ++p ) {
        printf(" process %ld name: ", p - x + 1);
        gets(p->name);
        printf(" %s's bt = ", p -> name);
        scanf("%d", &p -> bt);
        printf(" its arrival time: ");
        scanf("%d%*c", &p -> arrive);
        p -> rank = ++i; // here I'm givin' priority by default
    }
}

void display ( arr *x, int n ) {
    printf("\n name bt wt tt stay arive complete\n");
    for( arr *p = x; p < x + n; ++p )
        printf(" %4s %02d %02d %02d %4d %5d %02d\n", p -> name, p -> bt, p -> wt, p -> tt, p -> stay, p -> arrive, p -> cmplt);
}

void copyall ( arr *a, arr *b, int n ) {
    for ( arr *p = a, *q = b; p < a + n; ++p, ++q )
        *p = *q;
}

void copyWcalc ( arr *a, arr *b, int n ) {
    for ( arr *p = a, *q = b; p < a + n; ++p, ++q ) {
        p -> tt = (p -> wt = q -> wt) + p -> bt;
        p -> stay = (p -> cmplt = q -> cmplt) - p -> arrive;
    }
}

void calc ( arr *x, int n, int *cxsth ) {

    int i = 0, s = 0, save;
    arr *B;

    B = ( arr* ) malloc ( n * sizeof( arr ) );
    copyall(B, x, n);

    timesort( B, n );

    for ( int k = 0; k < n; ++k ) {

        if ( !B[k].bt ) // this here serves zero purpose in the code
            continue;   /* i added it at some point when i was trynna solve some problem
                            and now I'm just afraid to delete it */
        if ( !s )
            cxsth[1]++;

        s = 0;

        ibtsort(B, n, i);

        while( B[k].bt ) {
            s = 0;
            B[k].bt--;

            save = B[k].rank;

            for ( int e = k + 1; e < n; ++e )
                if (B[e].arrive <= i)
                    B[e].wt++;

            i++; // If you are curious this i represents the current time

            ibtsort(B, n, i);

            if ( save != B[k].rank ) {

                cxsth[1]++;
                s++;

                for ( int e = k; e < n; ++e )
                    if ( (save != B[e].rank) && (B[e].arrive <= i)  )
                        B[e].wt += cxsth[0];
            }
        }

        B[k].cmplt = i;
    }

    ranksort( B, n );
    copyWcalc( x, B, n );
}

void avrg ( arr *x, int n, double *aw, double *at, double *ar ) {
    for ( arr *p = x; p < x + n; ++p ) {

        *aw += p -> wt;
        *at += p -> tt;
        *ar += p -> stay;
    }

    *aw /= 1.0*n;
    *at /= 1.0*n;
    *ar /= 1.0*n;
}

void alloc ( arr **x, int n ){
    *x = ( arr* ) calloc ( n + 1, sizeof(arr) );
    if ( *x == NULL ) {
        printf(" memory contiguous allocation failed");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    int n, *cxsth = ( int* ) calloc ( 2, sizeof( int ) );
    arr *t;
    double aw = 0, at = 0, ar = 0; // average waiting, turnaround, response ( stay ) time

    printf(" Enter number of processes: ");
    scanf("%d%*c", &n);

    alloc(&t, n);

    printf(" Enter processes information : \n");
    read(t, n);
    printf(" context switching time?   ms\b\b\b\b");
    scanf("%d", cxsth);

    calc(t, n, cxsth);
    display(t, n);

    avrg(t, n, &aw, &at, &ar);
    printf("\n aw = %.2lf\n at = %.2lf\n ar = %.2lf\n number of switches %d", aw, at, ar, cxsth[1]);


    free(t);
    free(cxsth);
    t = NULL;
    cxsth = NULL;
    return 0;
}
