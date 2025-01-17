// for (int i = 0; i < n; i++)
//     for (int j = 0; j < n; j++)
//     {
//         cin >> adj[i][j];
//         if (adj[i][j])
//             ind[j]++;
//     }
// queue<int> q;
// for (int k = 0; k < n; k++)
// {
//     for (int i = 0; i < n; i++)
//     {
//         if (!ind[i])
//         {
//             cout << i << " ";
//             ind[i] = -1;
//             for (int j = 0; j < n; j++)
//             {
//                 if (adj[i][j])
//                 {
//                     ind[j]--;
//                 }
//             }
//             break;
//         }
//     }
// }