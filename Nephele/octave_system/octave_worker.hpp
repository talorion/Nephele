#ifndef TALORION_OCTAVE_WORKER_HPP
#define TALORION_OCTAVE_WORKER_HPP

#include <QObject>

namespace talorion {

    class octave_worker : public QObject
    {
        Q_OBJECT
    public:
        explicit octave_worker(QObject *par = 0);
        ~octave_worker();
        Q_DISABLE_COPY(octave_worker)

        void initialize();
        void dispose();

    signals:
        void eval_finished();

        void abort_all_dialoges();

    private slots:
        void slot_eval_function(const QString &script, bool debug=false);
        void slot_eval_file(const QString &script, bool debug=false);
        void debug_script_file(const QString &script);
        void slot_abort_script();

        void slot_register_octave_component(int ent);
    };

} // namespace talorion

#endif // TALORION_OCTAVE_WORKER_HPP
